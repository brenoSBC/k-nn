#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

const string training_dataset_path = "training_dataset.csv";

struct Data {

    double x{};
    double y{};
    int type{};
    int distance{};

    Data(double x, double y, int type) : x(x), y(y), type(type) {}

    Data(double x, double y) : x(x), y(y) {}

};

void read_dataset(const string& file_path, vector<Data>& datas)
{
    ifstream file(file_path);
    if(!file.is_open()) {
        cerr << "Error opening file: " + file_path << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    hash<string> hasher;
    
    while(getline(file, line, '\n')) {

        istringstream iss(line);
        string attribute;
        double x{}, y{};
        int type{};

        getline(iss, attribute, ',');
        x = stod(attribute);
        
        getline(iss, attribute, ',');
        y = stod(attribute);

        getline(iss, attribute, ',');
        type = hasher(attribute);

        datas.push_back(Data(x, y, type));
    }
    file.close();
}

double calculate_euclidian_distance(Data data, Data new_data) {
    return sqrt( (new_data.x - data.x) * (new_data.x - data.x) ) + ( (new_data.y - data.y) * (new_data.y - data.y) );
}

string knn(vector<Data>& datas, Data new_data, int K) {

    hash<string> hasher;

    for(int i = 0; i < datas.size(); i++) {
        double distance{calculate_euclidian_distance(datas[i], new_data)};
        datas[i].distance = distance;        
    }

    sort(datas.begin(), datas.end(), [](const Data& a, const Data& b) {
        return a.distance < b.distance;
    });

    int setosa{};
    int versicolor{};
    int virginica{};
    for(int j = 0; j < K; j++) {    
        if(datas[j].type == static_cast<int>(hasher("setosa"))) {
            setosa++;
        } else if(datas[j].type == static_cast<int>(hasher("versicolor"))) {
            versicolor++;
        } else if(datas[j].type == static_cast<int>(hasher("virginica"))) {
            virginica++;
        }
    }

    if(setosa > versicolor && setosa > virginica) return "setosa";
    if(versicolor > setosa && versicolor > virginica) return "versicolor";
    if(virginica > setosa && virginica > versicolor) return "virginica";

    return "nada";
}

int main(int argc, char **argv)
{

    vector<Data> datas;

    read_dataset(training_dataset_path, datas);   

    Data new_data(1, 1);

    string result = knn(datas, new_data, 1);
    cout << result << endl;

}