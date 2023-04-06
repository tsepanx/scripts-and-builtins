//
// Created by void on 3/8/22.
//

#include <iostream>
#include <vector>

using namespace std;

vector<string> split(string s, const string& delim) {
    auto result = vector<string>();

    s = s + delim;

    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delim)) != std::string::npos) {
        token = s.substr(0, pos);
        result.push_back(token);
        s.erase(0, pos + delim.length());
    }

    return result;
}

vector<vector<int>> parse_input(const string& input) {
    vector<vector<int>> res = vector<vector<int>>();
    auto input2 = input.substr(2, input.length() - 4);
//    cout << input << '\n';

    auto arrs = split(input2, "},{");

    for (auto & i : arrs) {
        vector<string> arr = split(i, ",");
        vector<int> r = vector<int>();
        for(int i = 0; i < arr.size(); ++i) {
            r.push_back(stoi(arr[i]));
        }
        res.push_back(r);
    }

    return res;
}

vector<vector<int>> get_reduced(vector<vector<int>> mat, int k, int q, int dim) {
    vector<vector<int>> temp = vector<vector<int>>(dim - 1, vector<int>(dim - 1, 0));
    int i = 0, j = 0;
    for (int row = 0; row < dim; row++) {
        for (int col = 0; col < dim; col++) {
            if (row != k && col != q) {
                temp[i][j++] = mat[row][col];
                if (j == dim - 1) {
                    j = 0;
                    i++;
                }
            }
        }
    }
    return temp;
}


int det(vector<vector<int>> matrix) {
    int dim = matrix.size();
    int dett = 0;
    if (dim == 1) { return matrix[0][0]; }
    else if (dim == 2) { return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]; }

    int sign = 1;

    for (int i = 0; i < dim; i++) {
        dett += sign * matrix[0][i] * det(get_reduced(matrix, 0, i, dim));
        sign *= -1;
    }
    return dett;
}

void print_vector(vector<vector<int>> v) {
    for (int i = 0; i < v.size(); ++i) {
        for (int j = 0; j < v[i].size(); ++j) {
            cout << v[i][j] << ' ';
        } cout << '\n';
    }
    cout << '\n';
}

int main() {
//    freopen("/home/void/DSA/DSA1/dsa_assignment2/input.txt", "rt", stdin);
//    freopen("/home/void/DSA/DSA1/dsa_assignment2/output.txt", "wt", stdout);
    freopen("input.txt", "rt", stdin);
    freopen("output.txt", "wt", stdout);

    string input; cin >> input;
    auto mat = parse_input(input);

    cout << det(mat) << '\n';
    return 0;
}