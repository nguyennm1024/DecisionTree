#pragma once
#ifndef DECISIONTREE_H_INCLUDED
#define DECISIONTREE_H_INCLUDED
#include<vector>
#include<math.h>
#include<string>
#include<fstream>
#include<sstream>
using namespace std;

//Training data

class TrainData
{
public:
	vector<vector<int> > Input;//Một dòng trên mỗi dòng
	vector<int> OutPut;//Cho phép các giá trị đầu ra riêng biệt
	void InSertData(vector<int> data, int out) //Nhập một hàng dữ liệu và đầu ra mục tiêu
	{
		Input.push_back(data);
		OutPut.push_back(out);
	}
};

int countLine(string filename)
{
	int numLines = 0;
	ifstream in(filename);
	//while ( ! in.eof() )
	string line;
	while (getline(in, line))
	{
		if (!in.eof())
		{
			++numLines;
		}
	}
	in.close();
	return numLines;
}
TrainData getData(const string& filename, int &last)
{
	int numLines = countLine(filename);
	ifstream infile;
	infile.open(filename);

	TrainData board;
	string line;
	int x;
	int test = 0;
	cout << numLines << " tuple" << endl;
	while (getline(infile, line))
	{
		vector<int> row;
		istringstream iss(line);
		while (iss >> x)
		{
			row.push_back(x);
			last = x;
		}
		row.pop_back();
		board.InSertData(row, last);
		test++;
		for (std::vector<int>::const_iterator i = row.begin(); i != row.end(); ++i)
			std::cout << *i << ' ';
		cout << last;
		cout << endl;
		if (test == numLines - 1)
		{
			break;
		}
	}
	infile.close();

	return board;
}
class Node
{
public:
	int Attribute;//Số thuộc tính
	bool IsLeaf;//Nó là một nút lá?
	vector<Node*> Num;//Nút con
	Node(int ID, bool a){
		this->Attribute = ID;
		this->IsLeaf = a;
	}

};

//Cây quyết định
class Tree
{
private:
	Node * Root;//Nút gốc

	vector<vector<int> > AttrData;//Danh sách thuộc tính

	Node* CreateTree(TrainData data, vector<int> usedAttr);//Thuật toán ID3 bao trùm cây
	int MostNormalOutPut(TrainData data);//Sử dụng đầu ra phổ biến nhất làm giá trị nút
	int Best(TrainData data, vector<int> usedAttr);//Tính toán thuộc tính với mức tăng thông tin cao nhất
	double Entropy(TrainData data);//Tính toán thông tin entropy
public:
	Tree();
	void GetOutPut();//Nhập một trường hợp để có được đầu ra
};



Tree::Tree()
{
	/*Nhap danh sach thuoc tinh va nhap so thuoc tinh phan loai cho tung thuoc tinh*/
	int stop = 0, num = 0;
	while (num < 299)
	{
		vector<int> temp;
		//cout<<"Attribute"<<"["<<num<<"]"<<":";
		//int aa;
		//cin>>aa;
		for (int i = 0; i<2; i++)
			temp.push_back(i);
		AttrData.push_back(temp);
		//cout<<"Stop?"<<endl;
		//cin>>stop;
		num++;
	}
	cout << "Done!\n";
	/*Nhap du lieu trainning va nhap so series thuoc tinh phan loai truc tiep theo thu tu*/
	TrainData data;
	stop = 0;
	//    while(!stop){
	//        vector<int> train;
	//        cout<<"TrainData:";
	//        int aa=0;
	//        for(unsigned int i=0;i<AttrData.size();i++){
	//            cin>>aa;
	//            train.push_back(aa);
	//        }
	//        cout<<"OutPut:";
	//        bool aaa;
	//        cin>>aaa;
	//        data.InSertData(train,aaa);
	//        cout<<"Stop?"<<endl;
	//        cin>>stop;
	//    }


	string fileName = "./Mymatrix.txt";
	int last;
	data = getData(fileName, last);
	vector<int> temp2;
	cout << "Trainning........." << endl;
	Root = CreateTree(data, temp2);
}

Node* Tree::CreateTree(TrainData data, vector<int> usedAttr)
{

	Node* root = new Node(0, 0);//Tạo nút gốc

								/*Nếu đầu ra là như nhau, tạo một nút với giá trị của đầu ra và một nút lá*/
	int stop = 1;
	for (unsigned int i = 1; i<data.OutPut.size(); i++)
	{
		if (data.OutPut[i] != data.OutPut[i - 1])
			stop = 0;
	}
	if (stop)
		return new Node(data.OutPut[0], 1);

	/*Nếu tất cả các thuộc tính được sử dụng, thì giá trị của nút trả về là đầu ra phổ biến nhất và là nút lá*/
	if (usedAttr.size() == AttrData.size())
		return new Node(MostNormalOutPut(data), 1);

	/*Chọn thuộc tính có mức tăng thông tin cao nhất làm nút*/
	int A = Best(data, usedAttr);
	usedAttr.push_back(A);//Lưu lại các thuộc tính đã sử dụng
	root->Attribute = A;

	/*Đệ quy tạo một cây mới theo từng thuộc tính thể loại*/
	for (unsigned int i = 0; i<AttrData[A].size(); i++)
	{
		TrainData tempExample;
		for (unsigned int j = 0; j<data.OutPut.size(); j++)
		{
			if (i == data.Input[j][A])
			{
				tempExample.InSertData(data.Input[j], data.OutPut[j]);
			}
		}
		if (tempExample.OutPut.empty())
		{
			root->Num.push_back(new Node(MostNormalOutPut(data), 1));
		}
		else
		{
			root->Num.push_back(CreateTree(tempExample, usedAttr));
		}
	}

	return root;
}


int Tree::MostNormalOutPut(TrainData data)
{
	vector<int> out;//Ghi lại output
	vector<int> count;//Số lượng
	for (unsigned int i = 0; i<data.OutPut.size(); i++)
	{
		bool ex = 0;
		int index = 0;
		for (unsigned int j = 0; j<out.size(); j++)
		{
			if (out[j] == data.OutPut[i])
			{
				ex = 1;
				index = j;
			}
		}
		if (ex)
		{
			count[index]++;
		}
		else
		{
			out.push_back(data.OutPut[i]);
			count.push_back(1);
		}
	}
	/*Số lượng nhiều nhất*/
	int maxi = 0;
	int max = 0;
	for (unsigned int i = 0; i<count.size(); i++)
	{
		if (count[i]>max)
		{
			maxi = i;
			max = count[i];
		}
	}
	return out[maxi];
}

double Tree::Entropy(TrainData data)
{
	/*Tính toán loại và số lượng output*/
	vector<double> out;
	vector<double> count;
	for (unsigned int i = 0; i<data.OutPut.size(); i++)
	{
		bool ex = 0;
		int index = 0;
		for (unsigned int j = 0; j<out.size(); j++)
		{
			if (out[j] == data.OutPut[i])
			{
				ex = 1;
				index = j;
			}
		}
		if (ex)
		{
			count[index]++;
		}
		else
		{
			out.push_back(data.OutPut[i]);
			count.push_back(1);
		}
	}
	/*Tính toán thông tin entropy*/
	double total = 0;
	for (unsigned int i = 0; i < count.size(); i++)
		total += count[i];
	double sum = 0;
	for (unsigned int i = 0; i<count.size(); i++)
	{
		double a = 0;
		if ((count[i] / total) != 0)
			a = log2((count[i] / total));
		sum -= (count[i] / total)*a;
	}
	return sum;
}

int Tree::Best(TrainData data, vector<int> usedAttr)
{
	vector<double>  Gain;//Ghi lại mức tăng thông tin cho mỗi thuộc tính

	bool used;
	/*Đặt mức tăng thông tin của thuộc tính đã sử dụng thành 0*/
	for (unsigned int i = 0; i<AttrData.size(); i++)
	{
		used = 0;
		for (unsigned int k = 0; k<usedAttr.size(); k++)
			if (i == usedAttr[k])
			{
				Gain.push_back(0.0);
				used = 1;
			}
		if (used)
			continue;
		/*Tính toán thông tin đạt được*/
		else
		{
			double es = Entropy(data);
			for (unsigned int j = 0; j<AttrData[i].size(); j++)
			{
				TrainData tempData;
				for (unsigned int k = 0; k<data.Input.size(); k++)
				{
					if (j == data.Input[k][i])
					{
						tempData.InSertData(data.Input[k], data.OutPut[k]);
					}
				}
				if (!tempData.Input.empty())
				{
					es -= (double(tempData.Input.size()) / double(data.Input.size()))*Entropy(tempData);
				}
			}
			Gain.push_back(es);
		}
	}

	/*Tính toán thuộc tính với mức tăng thông tin cao nhất*/
	int maxi = 0;
	double max = 0;
	for (unsigned int i = 0; i<Gain.size(); i++)
	{
		if (Gain[i]>max)
		{
			maxi = i;
			max = Gain[i];
		}
	}
	return maxi;
}

void Tree::GetOutPut()
{
	vector<int> data;
	//    cout<<"TestData:";
	//    int aa=0;
	//    for(int i=0; i<AttrData.size(); i++)
	//    {
	//        cin>>aa;
	//        data.push_back(aa);
	//    }

	ifstream in;
	in.open("./testData.txt");
	string line;
	int x;
	getline(in, line);
	istringstream iss(line);
	while (iss >> x)
	{
		data.push_back(x);
	}
	in.close();
	for (std::vector<int>::const_iterator i = data.begin(); i != data.end(); ++i)
		std::cout << *i << ' ';
	if (Root->IsLeaf)
	{
		cout << "OutPut:" << Root->Attribute << endl;
		return;
	}
	Node* current = Root->Num[data[Root->Attribute]];
	while (!current->IsLeaf) {
		current = current->Num[data[current->Attribute]];
	}
	cout << "OutPut:" << current->Attribute << endl;
	getchar();
}

#endif // PAIR