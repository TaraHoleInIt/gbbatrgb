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

#include <stdint.h>
#include <avr/common.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include "config.h"
#include "adc.h"

void Sleep( void ) {
    cli( );

    ADCSRA &= ~_BV( ADEN );
    power_adc_disable( );

    set_sleep_mode( SLEEP_MODE_IDLE );
    sleep_bod_disable( );

    sei( );
    sleep_mode( );
    sleep_disable( );

    cli( );
        power_adc_enable( );
        ADCSRA |= _BV( ADEN );
    sei( );

    ADC_Read( );
    ADC_Read( );
}

void DeepSleep( void ) {
    /* Disable interrupts before messing with WDT and sleep settings */
    cli( );

    /* Power off the ADC */
    ADCSRA &= ~_BV( ADEN );
    power_adc_disable( );

    wdt_reset( );

    /* Configure the watchdog timer to fire an interrupt */
    MCUCR = 0;
    _WD_CONTROL_REG = _BV( WDCE ) | _BV( WDE );
    _WD_CONTROL_REG = _BV( WDIE ) | _BV( WDE ) | Config_WDT_PS_BITS;

    set_sleep_mode( SLEEP_MODE_PWR_DOWN );
    sleep_bod_disable( );
    sleep_enable( );

    /* Re enable interrupts and go to sleep */
    sei( );
    sleep_mode( );

    /* Wake up here, disable sleep mode */
    sleep_disable( );

    /* Turn on the ADC and discard a few samples */
    cli( );
        power_adc_enable( );
        ADCSRA |= _BV( ADEN );
    sei( );

    ADC_Read( );
    ADC_Read( );
}
