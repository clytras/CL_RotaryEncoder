
/*
* Author: Christos Lytras <christos.lytras@gmail.com>
* Description: A helper class to attach encoder pins to interrupts for fast and smooth encoder rotation
* Version: 1.0
*/

#include "Arduino.h"
#include "CL_RotaryEncoder.h"

uint8_t CL_RotaryEncoder::m_pinA;
uint8_t CL_RotaryEncoder::m_pinB;
bool CL_RotaryEncoder::m_Bset = false;
bool CL_RotaryEncoder::m_Aset = false;

bool CL_RotaryEncoder::m_isRotating = false;
volatile int CL_RotaryEncoder::m_encoderPos = 1;

CL_RotaryEncoder::CL_RotaryEncoder(uint8_t pinA, uint8_t pinB) :
m_isEnabled(false),
m_lastPos(1),
m_cbOnLeft(NULL),
m_cbOnRight(NULL)
{
	m_pinA = pinA;
	m_pinB = pinB;
}

void CL_RotaryEncoder::enable()
{
	if (!m_isEnabled) {

		pinMode(m_pinA, INPUT);
		pinMode(m_pinB, INPUT);

		digitalWrite(m_pinA, HIGH);
		digitalWrite(m_pinB, HIGH);

		// encoder pin on interrupt 0 (pin 2)
		attachInterrupt(digitalPinToInterrupt(m_pinA), this->doEncoderA, CHANGE);
		// encoder pin on interrupt 1 (pin 3)
		attachInterrupt(digitalPinToInterrupt(m_pinB), this->doEncoderB, CHANGE);

		m_isEnabled = true;
	}
}

void CL_RotaryEncoder::disable()
{
	if (m_isEnabled) {
		detachInterrupt(digitalPinToInterrupt(2));
		detachInterrupt(digitalPinToInterrupt(3));
		m_isEnabled = false;
	}
}

void CL_RotaryEncoder::update()
{
	m_isRotating = true;  // reset the debouncer

	if (m_lastPos != m_encoderPos) {

		if (m_encoderPos > m_lastPos) {
			m_steps = m_encoderPos - m_lastPos;
			if (m_cbOnRight) m_cbOnRight(this, m_ptrDataArg);
		}
		else /*if (m_encoderPos < m_lastPos)*/ {
			m_steps = m_lastPos - m_encoderPos;
			if (m_cbOnLeft) m_cbOnLeft(this, m_ptrDataArg);
		}

		m_lastPos = m_encoderPos;
	}
}

void CL_RotaryEncoder::doEncoderA()
{
	// debounce
	if (m_isRotating) delay(1);  // wait a little until the bouncing is done

	// Test transition, did things really change? 
	if (digitalRead(m_pinA) != m_Aset) {  // debounce once more
		m_Aset = !m_Aset;

		// adjust counter + if A leads B
		if (m_Aset && !m_Bset)
		{
			m_encoderPos += 1;
		}

		m_isRotating = false;  // no more debouncing until loop() hits again
	}
}

void CL_RotaryEncoder::doEncoderB()
{
	if (m_isRotating) delay(1);

	if (digitalRead(m_pinB) != m_Bset) {
		m_Bset = !m_Bset;
		//  adjust counter - 1 if B leads A
		if (m_Bset && !m_Aset)
		{
			m_encoderPos -= 1;
		}

		m_isRotating = false;
	}
}
