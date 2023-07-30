# Gluttonous_snake
命令行贪吃蛇小游戏</br>

启动命令格式:</br>
\[地图宽] \[地图高] \[移动间隔] \[蛇头初始坐标X] \[蛇头初始坐标y] \[初始移动方向] \[最大食物数] \[获胜长度] \[随机数种子]</br>

备注:</br>
参数可以省略，省略的部分自动使用默认参数，多余、错误参数将被默认参数取代</br>
[地图宽]&emsp;&emsp;&emsp;&emsp;&emsp;默认值32&emsp;&emsp;不得小等于0&emsp;&emsp;&emsp;地图的宽度</br>
[地图高]&emsp;&emsp;&emsp;&emsp;&emsp;默认值18&emsp;&emsp;不得小等于0&emsp;&emsp;&emsp;地图的高度</br>
[移动间隔]&emsp;&emsp;&emsp;&emsp;默认值160&emsp;不得小等于0&emsp;&emsp;&emsp;代表蛇每次自动爬行之间间隔的毫秒数</br>
[蛇头初始坐标X]&emsp;&nbsp;默认值0&emsp;&emsp;不得超出地图边界&emsp;&emsp;小于0或大于等于[地图宽]则超出边界</br>
[蛇头初始坐标y]&emsp;&nbsp;默认值0&emsp;&emsp;不得超出地图边界&emsp;&emsp;小于0或大于等于[地图高]则超出边界</br>
[初始移动方向]&emsp;&emsp;默认值3&emsp;&emsp;取值只能为0 1 2 3&emsp;分别对应上 下 左 右</br>
[最大食物数]&emsp;&emsp;&emsp;默认值1&emsp;&emsp;不得小于0&emsp;&emsp;&emsp;只代表能同时出现的最大食物数，实际食物数可能小于最大食物数</br>
[获胜长度]&emsp;&emsp;&emsp;&emsp;默认值-1&emsp;&emsp;不得小于0(-1除外)&emsp;&emsp;即蛇长度大于等于其时玩家获胜，如果为-1则蛇必须填满地图玩家才能获胜</br>
[随机数种子]&emsp;&emsp;&emsp;默认值-1&emsp;&emsp;可以为任意值&emsp;&emsp;&emsp;如果为-1则每次随机开局，否则每次都是用固定随机数种子开局（即食物随机位置固定）</br>

示例参数:</br>
16 8 220 0 0 3 2 32 1</br>
地图宽高为16*8</br>
移动间隔为220ms</br>
蛇头初始位置为(0,0)且方向向右</br>
同时出现的最大食物数为2</br>
蛇长度达到32时即获胜</br>
每次都是用1作为随机数种子，食物位置每局固定</br>
