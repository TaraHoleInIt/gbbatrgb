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
#include <avr/io.h>
#include "config.h"
#include "adc.h"

void ADC_Init( void ) {
    /* Make sure ADC is not busy */
    while ( bit_is_set( ADCSRA, ADSC ) )
    ;

    cli( );
        /* Power on the ADC and use the 64x prescaler */
        ADCSRA = _BV( ADEN ) | _BV( ADPS2 ) | _BV( ADPS1 );
        ADCSRB = 0;

        /* Use 1.1v internal reference and ADC mux */
        //ADMUX = _BV( REFS1 ) | Pin_VBat;
        ADMUX = Config_ADMUX;

        /* Disable digital input buffers on VBat pin */
        DIDR0 = 0x1F & ~_BV( Pin_VBat );
    sei( );

    /* Discard a few samples */
    ADC_Read( );
    ADC_Read( );
    ADC_Read( );
    ADC_Read( );
}

uint16_t ADC_Read( void ) {
    uint8_t Lo = 0;
    uint8_t Hi = 0;

    /* Wait until ADC is not busy */
    while ( bit_is_set( ADCSRA, ADSC ) )
    ;

    /* Start ADC conversion */
    ADCSRA |= _BV( ADSC );

    while ( bit_is_set( ADCSRA, ADSC ) )
    ;

    /* Get result */
    Lo = ADCL;
    Hi = ADCH;

    return ( Hi << 8 ) | Lo;
}

uint16_t ADC_Sample( void ) {
    uint16_t Value = 0;

    Value+= ADC_Read( );
    Value+= ADC_Read( );
    Value+= ADC_Read( );
    Value+= ADC_Read( );

    return Value >> 2;
}
