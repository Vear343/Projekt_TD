#include<iostream>
using namespace std;

int Total_Currency = 0;

void Buy_T(int T_Price){ //ฟังค์ชั่นซื้อป้อม
    if (Total_Currency >= T_Price){ //เปรียบเทียบราคาว่าเงินพอหรือไม่
        Total_Currency -= T_Price; //เมื่อเงินพอ ลบเงินออกจากกองกลาง
        //place tower code.
        cout << "Paid : " << "\n";
        cout << "Now you have " << Total_Currency << " Baht left." << "\n"; //บอกจำนวนเงินที่เหลืออยู่
    }
    else { //เมื่อเงินไม่พอ
        cout << "Not Enough Money : ";
        cout << "Now you have only " << Total_Currency << " Baht." << "\n"; //บอกจำนวนเงินที่เหลืออยู่
    }
}

void Sell_T(int T_Price){ //ฟังค์ชั่นขายป้อม
    //delete tower code.
    Total_Currency += T_Price; //เพิ่มเงินเข้าไปในกองกลาง
    cout << "selled : ";
    cout << "Now you have " << Total_Currency << " Baht." << "\n"; //บอกจำนวนเงินที่เหลืออยู่
}
int main(){ //test
    cin >> Total_Currency;
    Buy_T(5); //ซื้อป้อมปืนครั้งแรกในราคา 5 Baht
    Sell_T(8); //ขายป้อมปืนในราคา 8 Baht
    Buy_T(10); 
}