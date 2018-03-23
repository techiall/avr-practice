- 将PC3配置为输出模式，输出高电平（使用位操作，只配置PC3，不影响其他引脚）

```c
DDRC |= (1 << 3);
PORT |= (1 << 3);
```

- PA3配置为输入模式，并打开其上拉电阻

```c
DDRA &= ~(1 << 3);
PORTA |= (1 << 3);
```

- 电路如下图所示，要求实现如下功能：当按键KEY1按下是D1-D3亮，其余LED灭  当案件KEY1释放是D1-D3灭，其余LED亮

![1](http://p.ananas.chaoxing.com/star3/origin/167fb1045a70b235182af48846f465af.png)

```c
int main(void)
{
        DDRC = 0x00;
        DDRD = 0x00;
        PORTD |= (1 << 3);
        while (1) {
                if (!(PIND & (1 << 3))) {
                        DDRC = 7;
                        while (!(PIND & (1 << 3)));
                        DDRC = ~DDRC;
                }
        }
        return 0;
}
```

- 电路如图所示，请问D3为普通红光发光二极管（LED），其正向导通电压为1.8V，
  - 如果要使其工作电流为3mA，请问其相应的限流电阻的阻值应为多少欧姆？
  - 编程实现如下功能，KEY1按下第一次，D3亮，按下第二次，D3灭，再按一下，D3亮，以此规律循环。请写出初始化以及功能代码，并在仿真环境中验证。

![1](http://p.ananas.chaoxing.com/star3/origin/167fb1045a70b235182af48846f465af.png)

```c
#include <iom16v.h>
#include <macros.h>

int main(void)
{
        DDRC = 0x00;
        DDRD = 0x00;
        PORTD |= (1 << 3);
        while (1) {
                if (!(PIND & (1 << 3))) {
                        DDRC ^= (1 << 3);
                        while (!(PIND & (1 << 3)));
                }
        }
        return 0;
}
```

- 请写出一个可用的延时函数？通过仿真软件进行验证。

```c
void delay(unsigned int time)
{
        int i;
        while (time--) {
                for (i = 0; i < 100; i++);
        }
}
```

