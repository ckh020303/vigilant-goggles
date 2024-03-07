C02Lite Controller Readme

1. 转发逻辑
   - 使用两个缓冲区接收CAN1和CAN2的数据, 分别为pCAN1RxBuffer和pCAN2RxBuffer;
   - CAN RX0中断处理程序中，将收到的数据分别写入缓冲区
   - Controller中调用 CanHub->Forwarder 中的 slot 函数，对两个缓冲区的数据依次处理和转发
   - DrvCAN 的 transmit 函数默认使用非block模式
2. 使用的关键问题