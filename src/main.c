/**
 * Copyright (C) 2019 Tara Keeling
 * 
 * This file is part of gbbatrgb.
 * 
 * gbbatrgb is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * gbbatrgb is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with gbbatrgb.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/common.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include "light_ws2812.h"
#include "config.h"
#include "adc.h"
#include "sleep.h"

#define MakecRGB( r, g, b ) ( struct cRGB ) { ( g / Config_BrightnessScale ), ( r / Config_BrightnessScale ), ( b / Config_BrightnessScale ) }
#define MakecRGBFromConfig( config ) MakecRGB( config )

#define ADCToMillivolts( ADC ) ( ( ( ( ( ( unsigned long ) ADC * 1000 ) / 1024 ) * Config_VRef ) / 1000 ) )
#define MillivoltsToADC( MVolts ) ( ( ( ( ( ( unsigned long ) MVolts * 1000 ) / Config_VRef ) * 1024 ) / 1000 ) )

static void IO_Init( void );
static void UpdateStatusLED( void );

struct cRGB PowerLED = { 0, 0, 0 };

/**
 * @brief Configures the internal pull ups on all unused pins
 * 
 */
static void IO_Init( void ) {
    PORTB = 0x1F & ~( _BV( Pin_LED ) | _BV( Pin_VBat ) );
    DDRB = _BV( Pin_LED );
}

/**
 * @brief Updates the LED colour depending on the voltage at VBat
 * 
 */
static void UpdateStatusLED( void ) {
    uint16_t ADCValue = ADCToMillivolts( ADC_Sample( ) ) * Config_VoltageDividerScale;

    switch ( ADCValue ) {
        case Config_Power_Good_Min ... Config_Power_Good_Max: {
            PowerLED = ( struct cRGB ) MakecRGBFromConfig( Config_Color_Good );
            break;
        }
        case Config_Power_Okay_Min ... Config_Power_Okay_Max: {
            PowerLED = ( struct cRGB ) MakecRGBFromConfig( Config_Color_Okay );
            break;
        }
        case Config_Power_Warn_Min ... Config_Power_Warn_Max: {
            PowerLED = ( struct cRGB ) MakecRGBFromConfig( Config_Color_Warn );
            break;
        }
        case Config_Power_Bad_Min ... Config_Power_Bad_Max:
        default: {
            PowerLED = ( struct cRGB ) MakecRGBFromConfig( Config_Color_Bad );
            break;
        }
    };

    ws2812_setleds( &PowerLED, 1 );
}

/**
 * @brief Watchdog timer interrupt handler
 * 
 */
ISR( WDT_vect ) {
    wdt_disable( );
}

int main( void ) {
    power_all_disable( );
    power_adc_enable( );

    IO_Init( );
    ADC_Init( );

    while ( true ) {
        UpdateStatusLED( );
        DeepSleep( );
    }

    return 0;
}
