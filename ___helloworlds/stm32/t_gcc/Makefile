# STM32 Makefile for Cortex-M3

CXX=arm-none-eabi-gcc
AR = arm-none-eabi-ar

OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# MCU specific
MCU = cortex-m3
TARGET_DEVICE = STM32F10X_MD



CXXFLAGS= -I./ -I ../include -I ./start  -I ./user  -I ./library \
			-Wall \
			-Os \
			-flto\
			
			
			
CXXFLAGS +=  -mcpu=$(MCU) -mthumb -mfloat-abi=soft
CXXFLAGS += -D$(TARGET_DEVICE) -DUSE_STDPERIPH_DRIVER


ASFLAGS = -mcpu=$(MCU) -mthumb


# Linker flags
LDFLAGS = -mcpu=$(MCU) -mthumb -mfloat-abi=soft
LDFLAGS += -specs=nano.specs -specs=nosys.specs
LDFLAGS += -Wl,--gc-sections -Wl,-Map=output.map
LDFLAGS += -flto 
LDFLAGS += -Os



# 在Makefile规则中，通配符会被自动展开。但在变量的定义和函数引用时，通配符将失效。
# 这种情况下如果需要通配符有效，就需要使用函数“wildcard”，它的用法是：$(wildcard PATTERN...) 。
# 在Makefile中，它被展开为已经存在的、使用空格分开的、匹配此模式的所有文件列表。
# 如果不存在任何符合此模式的文件，函数会忽略模式字符并返回空。
SOURCES_c += $(wildcard */*.c ) 
SOURCES_s += $(wildcard */*.s ) 


#去除所有的目录信息，DIR_SRC_GAME 里的文件名列表将只有文件名。
DIR_SRC_GAME = $(SOURCES_c)
DIR_SRC_GAME_s = $(SOURCES_s)



#模式替换‌ 功能，用于将文件名列表 DIR_SRC_GAME 从 .c 后缀批量替换为 .o 后缀
OBJS = $(DIR_SRC_GAME:%.c=%.o)

OBJS_s = $(DIR_SRC_GAME_s:%.s=%.o)

DEPS = $(DIR_SRC_GAME:%.c=%.d)

# Targets
TARGET_ELF = game.elf
TARGET_BIN = game.bin

log:
	@echo ">>> $(SOURCES_c)"
	@echo ">>> $(SOURCES_s)"
	@echo ">>> $(DIR_SRC_GAME)"
all : $(TARGET_BIN)

# include 前面加负号, 避免首次编译时依赖文件不存在的错误	 
-include $(DEPS)

#gcc -M 和 -MM 选项的主要区别在于‌是否包含系统头文件的依赖关系‌。
#核心区别：‌
#gcc -M：生成完整的依赖关系，包含‌所有头文件‌（用户自定义头文件和系统标准库头文件）
#gcc -MM：生成简化的依赖关系，‌忽略系统标准库头文件‌，只包含用户自定义头文件
#实际应用场景：
#	使用 -M 时，依赖关系会包含像 /usr/include/stdio.h 这样的系统头文件
#	使用 -MM 时，只会输出 main.o: main.c defs.h 这样的用户相关依赖
# $(DEPS) 目标， 针对头文件修改不会触发重新编译，如果修改了头文件，相关的 .c 文件不会自动重新编译，所以要引入相关头文件的规则
$(DEPS): %.d: %.c
	@$(CXX) -MM $(CXXFLAGS) $< > $@.tmp
	@sed 's,\(.*\)\.o[ :]*,\1.o $@ : ,g' < $@.tmp > $@
	@rm -f $@.tmp

	
	
# 原始的 .d文件 foo.o: foo.c bar.h  ===> sed 处理之后的为 foo.o foo.d : foo.c bar.h  
# 这样修改后，不仅 .o 文件依赖于 .c 和头文件，‌.d 文件也依赖于这些文件‌。
# 当头文件被修改时，不仅会触发 .o 文件重新编译，也会触发 .d 文件重新生成，确保依赖关系始终保持最新.
# 为什么要加这个 .d 依赖？ 
# 假设你新增了一个头文件 new.h 并在 foo.c 中包含它：
#	没有 foo.d 依赖：下次编译时，.d 文件仍然只记录旧的依赖关系，不会包含 new.h
#	有 foo.d 依赖：.d 文件会重新生成，自动包含 new.h 作为新依赖
#	这样设计能确保‌依赖关系始终与源代码保持同步‌，避免因依赖文件过时而导致的编译错误

#命令前的 @ 可以隐藏命令行打印输出
# sed -r 's/捕获内容/替换内容/' 文件, 上面用的是 / 做分隔符



# 静态模式规则‌，用于将多个 .c 源文件编译成对应的 .o 目标文件
# $(OBJS)：目标文件列表（如 main.o game.o）
# %.o: %.c：模式匹配，表示每个 .o 文件都依赖于同名的 .c 文件, 这是一个‌静态模式规则‌，只为 $(OBJS) 中列出的特定文件定义此模式. .O 对应 $(OBJS) 里的文件名列表, .c 对应 .o 的依赖
$(OBJS): %.o: %.c
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJS_s): %.o: %.s
	$(CXX) -c $(ASFLAGS) $< -o $@
	

#link
$(TARGET_ELF) : $(OBJS) $(OBJS_s)
#	$(AR) rc $@ $^
	$(CXX) -T stm32_linker_script.ld  -o $@  $(OBJS) $(OBJS_s) $(LDFLAGS)  


# Create binary
$(TARGET_BIN): $(TARGET_ELF)
	$(OBJCOPY) -O binary $< $@ 
	
 

	
clean:
	@rm -rf $(OBJS) $(DEPS) $(TARGET_BIN)  $(TARGET_ELF)


# .PHONY 的主要作用是‌声明伪目标‌，确保 Makefile 中的特定目标（如 clean、all）始终执行其命令，即使当前目录下存在同名文件也不会冲突
# 避免同名文件冲突，能让 Makefile 更易读，清晰表明这些目标不代表实际文件，而是用于执行特定操作（如编译、清理、安装）。
.PHONY: all clean log

	 