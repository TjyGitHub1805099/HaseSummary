在S32K中的ADC驱动adc_pal.c的中断函数ADC_S32K1xx_IrqHandler()中，IDE自动生成的代码，在调用应用层的回调函数时，参数不符合应用层要求，
固将其更改为：

	//(*(activeGroupCfg->callback))(&cbInfo, activeGroupCfg->callbackUserData);
        (*(activeGroupCfg->callback))(&cbInfo, (void*)instance);

以后如若adc_pal.c重新被生成，需要将其再次如此更改