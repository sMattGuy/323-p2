#include <iostream>
#include <stdlib.h>
#include <fstream>

class listNode{
	public:
	int data;
	listNode* next;
	//methods
	listNode(int data){
		this->data = data;
		this->next = nullptr;
	}
	std::string printNode(){
		if(this->next == nullptr){
			//special case for node that doesnt have a next to avoid seg fault
			std::string value = "("+std::to_string(this->data);
			value += ", NULL)";
			return value;
		}
		else{
			//normal case where there is a next node
			std::string value = "("+std::to_string(this->data);
			value += ", "+std::to_string(this->next->data);
			value +=	")->";
			return value;
		}
	}
};

class HTable{
	public:
	char op;
	int data;
	int bucketSize;
	listNode** hashTable;
	//methods
	HTable(int bucketSize){
		//store bucket size
		this->bucketSize = bucketSize;
		//init for hash table
		this->hashTable = new listNode*[bucketSize];
		for(int i=0;i<bucketSize;i++){
			this->hashTable[i] = new listNode(-9999);
		}
	}
	int hashInt(int data){
		int sumData = 0;
		for(int i=1;i<=5;i++){
			sumData += (data%10) * i;
			data = data/10;
		}
		return sumData%this->bucketSize;
	}
	void informationProcessing(std::ifstream& inFile, std::ofstream& outFile1, std::ofstream& outFile2){
		outFile2<<"Entered information processing method..."<<std::endl;
		//writes first character to objects char
		while(inFile >> this->op){
			//gets next data
			inFile >> this->data;
			outFile2<<"Operator: "<<this->op<<" Data: "<<this->data<<std::endl;
			int index = this->hashInt(this->data);
			outFile2<<"Index: "<<index<<std::endl;
			outFile1<<"Now printing bucket "<<index<<std::endl;
			this->printList(index, outFile1);
			if(this->op == '+'){
				hashInsert(index, this->data, outFile1, outFile2);
			}
			else if(this->op == '-'){
				hashDelete(index, this->data, outFile1, outFile2);
			}
			else if(this->op == '?'){
				hashRetrieval(index, this->data, outFile1, outFile2);
			}
			else{
				outFile1<<"Op is an unrecognized operation!"<<std::endl;
			}
			outFile1<<"After one op "<<this->op<<std::endl;
			this->printList(index, outFile1);
		}
	}
	listNode* findSpot(int index, int data){
		listNode* spot = this->hashTable[index];
		while(spot->next != nullptr && spot->next->data < data){
			spot = spot->next;
		}
		return spot;
	}
	void hashInsert(int index, int data, std::ofstream& outFile1, std::ofstream& outFile2){
		outFile2<<"Entered hash insert function..."<<std::endl;
		listNode* spot = this->findSpot(index, data);
		if(spot->next != nullptr && spot->next->data == data){
			outFile1<<"Data is already in the hash table, so no insertion takes place!"<<std::endl;
		}
		else{
			listNode* newNode = new listNode(data);
			newNode->next = spot->next;
			spot->next = newNode;
		}
		outFile2<<"Returning from hash insert function..."<<std::endl;
	}
	void hashDelete(int index, int data, std::ofstream& outFile1, std::ofstream& outFile2){
		outFile2<<"Entered hash delete function..."<<std::endl;
		listNode* spot = this->findSpot(index, data);
		if(spot->next != nullptr && spot->next->data == data){
			listNode* temp = spot->next;
			spot->next = temp->next;
			temp->next = nullptr;
			delete temp;
		}
		else{
			outFile1<<"Warning! The data is NOT in the database! Cannot delete!"<<std::endl;
		}
		outFile2<<"Returning from hash delete function..."<<std::endl;
	}
	void hashRetrieval(int index, int data, std::ofstream& outFile1, std::ofstream& outFile2){
		outFile2<<"Entered hash retrieval function..."<<std::endl;
		listNode* spot = this->findSpot(index, data);
		if(spot->next != nullptr && spot->next->data == data){
			outFile1<<"Yes, the data is in the hash table!"<<std::endl;
		}
		else{
			outFile1<<"No, the data is NOT in the hash table!"<<std::endl;
		}
		outFile2<<"Returning from hash retrieval function..."<<std::endl;
	}
	void printList(int index, std::ofstream& outFile1){
		listNode* printer = this->hashTable[index];
		std::string result = "hashTable["+std::to_string(index);
		result += "]: ";
		while(printer != nullptr){
			result += printer->printNode();
			printer = printer->next;
		}
		outFile1<<result<<std::endl;
	}
	void printHashTable(std::ofstream& outFile1){
		for(int i=0;i<this->bucketSize;i++){
			this->printList(i, outFile1);
		}
	}
};

int main(int argc, char** argv){
	//check arguments correct
	if(argc != 5){
		std::cout<<"You must include 4 arguments (inFile, bucketSize, outFile1, outFile2)"<<std::endl;
		return -1;
	}
	//read input file and check if readable
	std::ifstream inFile(argv[1]);
	if(!inFile.good()){
		std::cout<<"Invalid input file, please check the spelling!"<<std::endl;
		return -1;
	}
	inFile.close();
	//check bucket size is correct
	int bucketSize = atoi(argv[2]);
	if(bucketSize == 0){
		std::cout<<"Invalid bucket size!"<<std::endl;
		return -1;
	}
	//init for out files
	std::ofstream outFile1;
	std::ofstream outFile2;
	//creation of hashtable
	HTable hashTable(bucketSize);
	//process data
	inFile.open(argv[1]);
	outFile1.open(argv[3],std::ios::out | std::ios::app);
	outFile2.open(argv[4],std::ios::out | std::ios::app);
	hashTable.informationProcessing(inFile,outFile1,outFile2);
	hashTable.printHashTable(outFile1);
	//close all files
	inFile.close();
	outFile1.close();
	outFile2.close();
	return 0;
}