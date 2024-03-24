#pragma once

class Field
{
private:
	int value; // 0 - 8 wartosci pola 9 - mina
	bool clicked;
	bool flag;
public:
	Field();
	~Field();
	
	int getValue() {return value;}
	bool isClicked() {return clicked;}
	bool isFlag() {return flag;}

	void setValue(int value) {Field::value = value;}
	void setClicked(bool clicked) {Field::clicked = clicked;}
	void setFlag(bool flag) {Field::flag = flag;}
};