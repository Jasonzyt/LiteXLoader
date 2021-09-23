## 💰 经济系统事件

此处为LXL内置经济系统相关的经济变动事件

#### `"onMoneyAdd"` - 玩家金额增加

- 监听函数原型
  `function(xuid,money)`
- 参数：
  - xuid : `String`  
    金额变动的玩家的Xuid
  - money : `Integer`  
    增加的金额
- 拦截事件：函数返回`false`

<br>

#### `"onMoneyReduce"` - 玩家金额减少

- 监听函数原型
  `function(xuid,money)`
- 参数：
  - xuid : `String`  
    金额变动的玩家的Xuid
  - money : `Integer`  
    减少的金额
- 拦截事件：函数返回`false`

<br>

#### `"onMoneyTrans"` - 玩家转账

- 监听函数原型
  `function(from,to,money)`
- 参数：
  - from : `String`  
    发起转账的玩家的Xuid
  - to : `String`  
    接受转账的玩家的Xuid
  - money : `Integer`  
    转账的金额
- 拦截事件：函数返回`false`

<br>

#### `"onMoneySet"` - 直接设置玩家金额

- 监听函数原型
  `function(xuid,money)`
- 参数：
  - xuid : `String`  
    金额变动的玩家的Xuid
  - money : `Integer`  
    被设置的金额
- 拦截事件：函数返回`false`

<br>