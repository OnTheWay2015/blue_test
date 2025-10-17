<script setup>
import { ref } from 'vue'
import { testIncome } from '../scripts/testInCome'

let configstr = `
{
    "static_income":9.8,
    "auto_add_member":false,
"players":
[
     {        "ID":1,        "parentID":0,        "vip":6    }
    ,{        "ID":2,        "parentID":1,        "vip":5    }
    ,{        "ID":3,        "parentID":2,        "vip":4    }
    ,{        "ID":4,        "parentID":3,        "vip":3    }
    ,{        "ID":5,        "parentID":4,        "vip":3    }
    ,{        "ID":6,        "parentID":5,        "vip":1    }
    ,{        "ID":7,        "parentID":6,        "vip":5    }
    ,{        "ID":8,        "parentID":7,        "vip":3    }
    ,{        "ID":9,        "parentID":8,        "vip":2    }
    ,{        "ID":10,        "parentID":9,        "vip":0    }
]
}

`;

const inputValue = ref('')
const submittedValue = ref('')
const res = ref('')

inputValue.value = configstr 

const handleSubmit = () => {
  if (inputValue.value.trim()) {
    let calc = new testIncome( );
    res.value = calc.startCalc(inputValue.value);
    //submittedValue.value = inputValue.value
  }
}

</script>

<template>

<textarea 
      v-model="inputValue"
      placeholder="请输入多行文本..."
      @input="handleInput"
      rows="10"
      cols="200"
    ></textarea>


    <button 
      :disabled="!inputValue"
      @click="handleSubmit"
    >
     计算 
    </button>

 <textarea rows="30" cols="200" >
{{ res }}
</textarea>
</template>


<style scoped>
.input-container {
  display: flex;
  gap: 10px;
  margin: 20px;
}
textarea{
  padding: 8px;
  border: 1px solid #ccc;
  border-radius: 4px;
}
input {
  padding: 8px;
  border: 1px solid #ccc;
  border-radius: 4px;
}
button {
  padding: 8px 16px;
  background-color: #42b983;
  color: white;
  border: none;
  border-radius: 4px;
  cursor: pointer;
}
button:disabled {
  background-color: #cccccc;
  cursor: not-allowed;
}
</style>
