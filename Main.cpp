#include <Windows.h>
#include <iostream>
#include <time.h>

using namespace std;

int rows = 4;
int cols = 4;

int determinant(int** matrix);
void adjoint(int** src, float** dst);

template <typename T>
ostream& operator<<(ostream& os, T** mat) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++)
			os << mat[i][j] << '\t';
		os << endl;
	}
	return os;
}

int main() {
	int det;

	srand(time(NULL));
	
	int** matrix = new int*[rows]; //원본
	for (int i = 0; i < rows; i++)
		matrix[i] = new int[cols];

	float** adjo = new float*[rows]; // 딸림 행렬 및 역행렬
	for (int i = 0; i < rows; i++)
		adjo[i] = new float[cols];

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			matrix[i][j] = rand() % 3;

	cout << "original ------" << endl;
	cout << matrix << endl;

	det = determinant(matrix);

	if (det == 0) {
		cout << "역행렬이 존재하지 않음" << endl;
		return 1;
	}
	cout << "determinant ------" << endl;
	cout << det << endl;

	adjoint(matrix, adjo);

	cout << "adjoint ------" << endl;
	cout << adjo << endl;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			adjo[i][j] /= det;

	cout << "inverse ------" << endl;
	cout << adjo << endl;

	for (int i = 0; i < rows; i++)
		delete[] matrix[i];
	delete[] matrix;

	for (int i = 0; i < rows; i++)
		delete[] adjo[i];
	delete[] adjo;

	return 0;
}

int determinant(int** matrix) {
	int result = 0;
	int rdmul[4] = { 0, };// A_1j 곱한 값들...
	int thmul[4][3] = { 0, };
	int thdel[4][3][3];

	int t_x, t_y = 0;
	for (int i = 0; i < 4; i++) {
		t_x = 0; t_y = 0;

		if (i % 2 == 0) rdmul[i] = matrix[0][i];
		else rdmul[i] = -matrix[0][i];

		for (int j = 1; j < 4; j++) {
			t_x = 0;
			for (int k = 0; k < 4; k++)
				if (k != i) {
					thdel[i][t_y][t_x] = matrix[j][k];
					t_x++;
				}
			t_y++;
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			int tmp = 1;
			if (j % 2 == 0) tmp *= thdel[i][0][j];
			else tmp *= -thdel[i][0][j];
			switch (j)
			{
			case 0:
				tmp *= (thdel[i][1][1] * thdel[i][2][2] - thdel[i][1][2] * thdel[i][2][1]);
				break;
			case 1:
				tmp *= (thdel[i][1][0] * thdel[i][2][2] - thdel[i][1][2] * thdel[i][2][0]);
				break;
			case 2:
				tmp *= (thdel[i][1][0] * thdel[i][2][1] - thdel[i][1][1] * thdel[i][2][0]);
				break;
			default:
				break;
			}
			thmul[i][j] = tmp;
		}
	}

	for (int i = 0; i < 4; i++) {
		int tmpsum = 0;
		for (int j = 0; j < 3; j++)
			tmpsum += thmul[i][j];
		result += rdmul[i] * tmpsum;
	}

	return result;
}

void adjoint(int** src, float** dst) {
	int tmparr[3][3] = { 0, };
	int confact[4][4] = { 0, };
	int tmpres, tmpmid;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			tmpmid = 0;
			if ((i + j) % 2 == 0) tmpres = 1;
			else tmpres = -1;
			int t_x = 0, t_y = 0;
			for (int k = 0; k < 4; k++) {
				t_x = 0;
				if (k == i)
					continue;
				for (int l = 0; l < 4; l++) {
					if (l != j) {
						tmparr[t_y][t_x] = src[k][l];
						t_x++;
					}
				}
				t_y++;
			}
			t_y = 0;
			for (int k = 0; k < 3; k++) {
				int tmp_small;
				if (k % 2 == 0) tmp_small = tmparr[0][k];
				else tmp_small = -tmparr[0][k];
				switch (k)
				{
				case 0:
					tmp_small *= tmparr[1][1] * tmparr[2][2] - tmparr[1][2] * tmparr[2][1];
					break;
				case 1:
					tmp_small *= tmparr[1][0] * tmparr[2][2] - tmparr[1][2] * tmparr[2][0];
					break;
				case 2:
					tmp_small *= tmparr[1][0] * tmparr[2][1] - tmparr[1][1] * tmparr[2][0];
					break;
				default:
					break;
				}
				tmpmid += tmp_small;
			}
			tmpres *= tmpmid;
			confact[i][j] = tmpres;
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			dst[i][j] = confact[j][i];
		}
	}

	return;
}