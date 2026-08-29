#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

enum class Type {
    Unknown,
    Setosa,
    Versicolor,
    Virginica,
};

struct Data {

    double x{};
    double y{};
    double z{};
    double w{};
    Type type{};
    double distance{};

    Data(double x, double y, double z, double w, Type type) : x(x), y(y), z(z), w(w), type(type) {}

    Data(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}

};

void read_training_dataset(const string& file_path, vector<Data>& datas)
{
    ifstream file(file_path);
    if(!file.is_open()) {
        cerr << "Error opening file: " + file_path << endl;
        exit(EXIT_FAILURE);
    }

    string line;    
    while(getline(file, line, '\n')) {

        istringstream iss(line);
        string attribute;
        double x{}, y{}, z{}, w{};
        Type type{};

        getline(iss, attribute, ',');
        x = stod(attribute);
        
        getline(iss, attribute, ',');
        y = stod(attribute);
        
        getline(iss, attribute, ',');
        z = stod(attribute);

        getline(iss, attribute, ',');
        w = stod(attribute);

        getline(iss, attribute, ',');
        type = (attribute == "setosa") ? Type::Setosa : (attribute == "versicolor") ? Type::Versicolor : Type::Virginica;

        datas.push_back(Data(x, y, z, w, type));
    }
    file.close();
}

void read_test_dataset(const string& file_path, vector<Data>& new_datas) 
{
    ifstream file(file_path);
    if(!file.is_open()) {
        cerr << "Error opening file: " + file_path << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    while(getline(file, line, '\n')) {

        istringstream iss(line);
        string attribute;
        double x{}, y{}, z{}, w{};

        getline(iss, attribute, ',');
        x = stod(attribute);
            
        getline(iss, attribute, ',');
        y = stod(attribute);

        getline(iss, attribute, ',');
        z = stod(attribute);

        getline(iss, attribute, ',');
        w = stod(attribute);

        new_datas.push_back(Data(x, y, z, w));
    }
    file.close();
}

double calculate_euclidian_distance(Data data, Data new_data) {
    return sqrt( 
        (new_data.x - data.x) * (new_data.x - data.x) + 
        (new_data.y - data.y) * (new_data.y - data.y) + 
        (new_data.z - data.z) * (new_data.z - data.z) + 
        (new_data.w - data.w) * (new_data.w - data.w) 
    );
}

void knn(vector<Data>& datas, vector<Data>& new_datas, int K) {

    for(Data& new_data : new_datas) {
        for(Data& data : datas) {
            data.distance = calculate_euclidian_distance(data, new_data);
        }

        sort(datas.begin(), datas.end(), [](const Data& a, const Data& b) {
            return a.distance < b.distance;
        });

        int setosa{};
        int versicolor{};
        int virginica{};
        for(int j = 0; j < new_datas.size(); j++) {
            if(datas[j].type == Type::Setosa) {
                setosa++;
            } else if(datas[j].type == Type::Versicolor) {
                versicolor++;
            } else {
                virginica++;
            }
        }
        
        if (setosa >= versicolor && setosa >= virginica) {
            new_data.type = Type::Setosa;
        } else if (versicolor >= virginica) {
            new_data.type = Type::Versicolor;
        } else {
            new_data.type = Type::Virginica;
        }
    }
}


int main(int argc, char **argv)
{
    int K{3};
    const string training_dataset_path = "datasets/training_dataset.csv";
    const string test_dataset_path     = "datasets/test_dataset.csv";
    
    vector<Data> datas;
    vector<Data> new_datas;

    read_training_dataset(training_dataset_path, datas);   
    read_test_dataset(test_dataset_path, new_datas);

    knn(datas, new_datas, K);
    
    for(auto i : new_datas) {
        if(i.type == Type::Setosa) {
            cout << "setosa" << endl;
        } else if(i.type == Type::Versicolor) {
            cout << "versicolor" << endl;
        } else {
            cout << "virginica" << endl;
        }
    }
}