#ifndef _CONFIG_H_
#define _CONFIG_H_

#define Pin_LED PINB2
#define Pin_VBat PINB3

#define Config_BrightnessScale 16

#define Config_VRef 1100
#define Config_VoltageDividerScale 6

#define ADMUX_AVCC_ADC3 ( Pin_VBat )
#define ADMUX_BG1_1_ADC3 ( _BV( REFS1 ) | Pin_VBat )

#define Config_ADMUX ADMUX_BG1_1_ADC3

#define Config_WDT_PS_BITS _BV( WDP3 ) 

#define Config_Power_Good_Max 9999
#define Config_Power_Good_Min 4200

#define Config_Power_Okay_Max 4199
#define Config_Power_Okay_Min 3900

#define Config_Power_Warn_Max 3899
#define Config_Power_Warn_Min 3600

#define Config_Power_Bad_Max 3599
#define Config_Power_Bad_Min 0

#define Config_Color_Good 0, 255, 255
#define Config_Color_Okay 255, 255, 0
#define Config_Color_Warn 255, 0, 255
#define Config_Color_Bad 255, 0, 0

#endif
