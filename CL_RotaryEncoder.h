
/*
* Author: Christos Lytras <christos.lytras@gmail.com>
* Description: A helper class to attach encoder pins to interrupts for fast and smooth encoder rotation
* Version: 1.0
*/

#ifndef __CL_ROTARYENCODER__
#define __CL_ROTARYENCODER__


class CL_RotaryEncoder {

public:
	typedef void(*change_callback)(CL_RotaryEncoder*, void*);

	CL_RotaryEncoder(uint8_t pinClk, uint8_t pinDIO);

	int resetValue() { m_encoderPos = 0; }
	int setValue(int value) { m_encoderPos = value; }
	int getValue() { return m_encoderPos;  }
	int getEncoderPos() { return m_encoderPos;  }
	int getSteps() { return m_steps; }
	void setDataArg(void* ptrDataArg) { m_ptrDataArg = ptrDataArg; }

	void enable();
	void disable();
	void update();

	void onLeft(change_callback onLeftCallback) { m_cbOnLeft = onLeftCallback; }
	void onRight(change_callback onRightCallback) { m_cbOnRight = onRightCallback;  }

	static void doEncoderA();
	static void doEncoderB();

	static uint8_t m_pinA;
	static uint8_t m_pinB;
	static bool m_Bset;
	static bool m_Aset;

	static bool m_isRotating;
	static volatile int m_encoderPos;

private:
	int m_lastPos;
	int m_steps;
	bool m_isEnabled;
	void* m_ptrDataArg;

	change_callback m_cbOnLeft;
	change_callback m_cbOnRight;
};

#endif // __CL_ROTARYENCODER__
