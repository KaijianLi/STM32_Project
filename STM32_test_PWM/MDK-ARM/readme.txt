本程序的实现效果是在PB1引脚上输出PWM波，
PWM波的频率为2000Hz；计算公式为 ：
			f（PWM） = 72M / [（PSC + 1）* （ARR + 1）]
在 CubeMX 中配置的 PSC 为 72 - 1，ARR 为 499 - 1
PWM波的占空比从0.002 到 1 递增，然后在递减到 0.002 间隔时间为 10ms
占空比的调节即为 CubeMX 中对 Pulse 的修改值，然后在 while 循环中通过 
			htim3.Instance -> CCR4
来动态修改 比较值，实现占空比的增加。