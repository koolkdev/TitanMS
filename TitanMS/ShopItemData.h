#ifndef SHOPITEMDATA_H
#define SHOPITEMDATA_H

#include "DataStruct.h"

class ShopItemData : public DataStruct {
private:
	int price;
public:
	ShopItemData(){
		id=-1;
		price=1;
	}

	void setPrice(int price){
		this->price = price;
	}
	int getPrice(){
		return price;
	}

};

#endif