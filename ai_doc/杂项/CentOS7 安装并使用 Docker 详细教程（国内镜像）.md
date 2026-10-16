# CentOS7 安装并使用 Docker 详细教程（国内镜像）

# 一、环境说明

- **操作系统**：CentOS 7\.x（7\.6/7\.9 通用）

- **安装版本**：Docker CE 社区免费版

- **网络环境**：全部使用国内阿里云镜像，解决国外源超时、拉取失败问题

- **权限要求**：root 用户 或 拥有sudo权限用户

# 二、前置准备（必做）

## 2\.1 关闭防火墙与SELinux（避免拦截容器）

```bash
# 永久关闭SELinux
setenforce 0
sed -i 's/^SELINUX=enforcing/SELINUX=disabled/' /etc/selinux/config

# 关闭防火墙（生产环境可只放行端口）
systemctl stop firewalld
systemctl disable firewalld
```

## 2\.2 更换阿里云YUM源（CentOS7专用）

```bash
# 备份原有官方源
mv /etc/yum.repos.d/CentOS-Base.repo /etc/yum.repos.d/CentOS-Base.repo.bak

# 下载阿里国内源
curl -o /etc/yum.repos.d/CentOS-Base.repo https://mirrors.aliyun.com/repo/Centos-7.repo

# 刷新缓存
yum clean all && yum makecache
```

## 2\.3 卸载旧版本Docker

防止旧版本冲突，执行卸载命令，无残留则无输出，属于正常现象。

```bash
yum remove -y docker \
docker-client \
docker-client-latest \
docker-common \
docker-latest \
docker-latest-logrotate \
docker-logrotate \
docker-engine
```

# 三、安装Docker

## 3\.1 安装依赖工具

```bash
yum install -y yum-utils device-mapper-persistent-data lvm2
```

## 3\.2 添加阿里云Docker源

```bash
yum-config-manager --add-repo https://mirrors.aliyun.com/docker-ce/linux/centos/docker-ce.repo
```

## 3\.3 安装Docker稳定版

```bash
yum install -y docker-ce docker-ce-cli containerd.io
```

## 3\.4 启动Docker并设置开机自启

```bash
# 启动docker
systemctl start docker
# 设置开机自启
systemctl enable docker
# 查看运行状态
systemctl status docker
```

## 3\.5 验证安装是否成功

```bash
docker --version
```

出现版本号即为安装成功，示例：`Docker version 25\.0\.3, build 4debf41`

# 四、配置国内镜像加速（重中之重）

CentOS7必须配置镜像加速，否则拉取官方镜像超时、失败。

```bash
# 创建docker配置文件
mkdir -p /etc/docker
tee /etc/docker/daemon.json <<-'EOF'
{
  "registry-mirrors": [
    "https://docker.mirrors.ustc.edu.cn",
    "https://hub-mirror.c.163.com"
  ]
}
EOF

# 重载配置并重启docker
systemctl daemon-reload
systemctl restart docker
```

## 4\.1 检查镜像加速是否生效

```bash
docker info
```

看到国内镜像地址，说明配置成功。

# 五、Docker常用基础命令

## 5\.1 基础操作命令

```bash
# 启动docker
systemctl start docker
# 停止docker
systemctl stop docker
# 重启docker
systemctl restart docker
```

## 5\.2 镜像操作

```bash
# 查看本地镜像
docker images
# 搜索镜像
docker search 镜像名
# 拉取镜像
docker pull 镜像名:版本
# 删除镜像
docker rmi 镜像ID
```

## 5\.3 容器操作

```bash
# 查看正在运行容器
docker ps
# 查看所有容器（包含停止）
docker ps -a
# 删除容器
docker rm 容器ID
# 重启容器
docker restart 容器ID
# 查看容器日志
docker logs -f 容器名
```

# 六、测试Docker（运行测试容器）

```bash
# 运行hello-world测试容器
docker run hello-world
```

出现英文欢迎字样，说明Docker环境完全正常，网络拉取无问题。

# 七、卸载Docker

```bash
# 停止服务
systemctl stop docker
# 卸载程序
yum remove -y docker-ce docker-ce-cli containerd.io
# 删除残留文件
rm -rf /var/lib/docker
rm -rf /etc/docker
```

# 八、常见问题解决

### 8\.1 报错：yum源超时

解决：已更换阿里源，重复执行 `yum clean all \&amp;\&amp; yum makecache` 刷新缓存。

### 8\.2 拉取镜像缓慢、超时

解决：确认已经配置上文**国内镜像加速**，重启Docker生效。

### 8\.3 CentOS7 提示内核过低

解决：CentOS7默认内核满足Docker运行，无需升级，忽略无关警告。

> （注：文档部分内容可能由 AI 生成）
