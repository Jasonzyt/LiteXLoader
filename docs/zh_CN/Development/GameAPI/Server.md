## 💻 服务端设置 API

下面这些API提供了自定义某些服务器设置的接口

### 获取BDS服务端版本号

`mc.getBDSVersion()`

- 返回值：服务端版本号字符串，格式形如`v1.17.10`
- 返回值类型：`String`

<br>

### 设置服务器Motd字符串  

`mc.setMotd(motd)`

- 参数：
  - motd : `String`  
    目标Motd字符串  
- 返回值：是否设置成功
- 返回值类型：`Boolean`

<br>
