/*
 * Main.cpp
 *
 *  Created on: Jan 27, 2022
 *      Author: firasj
 */

#include <iostream>
#include <string>
#include "LinkedHashMap.h"
#include <optional>
using namespace std;

typedef string K;
typedef string V;


int main(){

	LinkedHashMap<K, V, 5> map;
	cout<<"adding"<<endl;
    map.put("John Smith", "5211234");
    cout<<map.toString()<<endl;

    cout<<"adding"<<endl;
    map.put("Lisa Smith", "5218976");
    cout<<map.toString()<<endl;

    cout<<"adding"<<endl;
    map.put("Ted Baker", "4184165");
    cout<<map.toString()<<endl;

    cout<<"adding"<<endl;
    map.put("Sam Doe", "5215030");

    cout<<"adding"<<endl;
    map.put("Sandra Dee", "5219655");
    cout<<map.toString()<<endl;

//    map.put(NULL, NULL);
    cout<<"adding"<<endl;
    map.put("", "");
    cout<<map.toString()<<endl;

    cout<<"Updating entry of key: ''"<<endl;
    map.put("", "Hello");
    cout<<map.toString()<<endl;

    cout<<"removing key '' "<<endl;
    map.remove("");
    cout<<map.toString()<<endl;

    cout<<"Value of key 'John Smith' "<<endl;
    optional<V> value = map.get("John Smith");

    if (value.has_value()){
        cout<<value.value()<<endl;
    }else{
    	cout<<"No values"<<endl;
    }

    cout<<map.toString()<<endl;

    cout<<"removing key 'John Smith' "<<endl;
    map.remove("John Smith");
    cout<<map.toString()<<endl;

	return 0;
}



