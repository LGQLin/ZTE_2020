#include <unordered_map>
#include <bits/stdc++.h>
using namespace std;

bool file_input(const char* filename, unordered_map<int, vector<vector<int>>>& id3_map, vector<vector<int>>& data, int mod);
void group(vector<vector<int>>& j_arr, vector<vector<int>>& ans);

const char* data1_path = "/home/web/ztedatabase/input1.csv";
const char* data2_path = "/home/web/ztedatabase/input2.csv";
///const char* data1_path = "input1_1.csv";
//const char* data2_path = "input2_1.csv";

int main()
{
	//读文件、到结构
	unordered_map<int, vector<vector<int>>> id3_map;
	vector<vector<int>> j_arr;
	if (file_input(data1_path, id3_map,j_arr, 1))
	{
		if (!file_input(data2_path, id3_map, j_arr, 2))
		{
			cout << "file open erro!" << endl;
			return 0;
		}		
	}
	id3_map.clear();
	//分组
	vector<vector<int>> ans(j_arr.size(),vector<int>(4));
	group(j_arr, ans);
	j_arr.clear();
	//输出
	for (auto& x : ans)	
		printf("%d,%d\n", x[0], x[1]);
	
	//cout << ans.size() << endl;
	return 0;
}
void data_analyse(char* buf, uint32_t& len, unordered_map<int, vector<vector<int>>>& id3_map, vector<vector<int>>& data, int &mod)
{
	vector<vector<int>> temp(0);
	vector<int> lineArray(2), mergearr(4);
	int32_t res;
	int32_t num_front = 0, num_back = 0;
	bool negative = false;
	char c;
	while (num_front < len)
	{
		//读取第一个数据
		res = 0;
		if (buf[num_front] == '-')
		{
			negative = true;
			num_front++;
			num_back = num_front;
		}
		while (buf[num_front] != ',')
			num_front++;
		while (num_back < num_front)
			res = res * 10 + buf[num_back++] - '0';
		if (negative)
		{
			lineArray[0] = -res;
			negative = false;
		}
		else
			lineArray[0] = res;
		num_back = ++num_front;
		//读取第二个数据
		res = 0;
		if (buf[num_front] == '-')
		{
			negative = true;
			num_front++;
			num_back = num_front;
		}
		while (buf[num_front] != ',')
			num_front++;
		while (num_back < num_front)
			res = res * 10 + buf[num_back++] - '0';
		if (negative)
		{
			lineArray[1] = -res;
			negative = false;
		}
		else
			lineArray[1] = res;
		num_back = ++num_front;
		//读取第三个数据
		res = 0;
		if (buf[num_front] == '-')
		{
			negative = true;
			num_front++;
			num_back = num_front;
		}
		while (buf[num_front] != '\n')
			num_front++;
		while (num_back < num_front)
			res = res * 10 + buf[num_back++] - '0';
		if (negative)
		{
			res = -res;
			negative = false;
		}
		num_front++;
		//将数据压入数据结构
		if (mod == 1) //处在解析文件1模式
		{
			if (id3_map.find(res) == id3_map.end())	//表示没有加入过
			{
				id3_map.insert(pair<int, vector<vector<int>>>(res, temp));
				id3_map[res].emplace_back(lineArray);
			}
			else
				id3_map[res].emplace_back(lineArray);
		}
		else if (mod == 2)	//处在解析文件2模式
		{
			if (id3_map.find(res) != id3_map.end()) //表示id3有值
			{
				for (auto& x : id3_map[res])
				{					
					mergearr[0] = x[0];				//文件1的 id1
					mergearr[1] = x[1];				//文件1的 id2
					mergearr[2] = lineArray[0];		//文件2的 id1
					mergearr[3] = lineArray[1];		//文件2的 id2
					data.emplace_back(mergearr);	//将合并后的记录转存到data中
				}
			}
		}		
		while (num_front < len)
		{
			c = buf[num_front];
			if (isdigit(c) || c == '-')
				break;
			num_front++;
		}
		num_back = num_front;
	}
}
bool file_input(const char* filename,unordered_map<int,vector<vector<int>>> &id3_map ,vector<vector<int>>& data, int mod)
{
	FILE* stream = NULL;
	uint32_t char_num, len;
	char* p = NULL;
	stream = freopen(filename, "r", stdin);
	if (stream)
	{
		fseek(stream, 0, SEEK_END);
		char_num = ftell(stream);
		fseek(stream, 0, SEEK_SET);

		p = new char[char_num];
		len = fread(p, 1, char_num, stdin);

		data_analyse(p, len, id3_map, data, mod);
		delete[] p;
		p = NULL;
		fclose(stream);
		return true;
	}
	else
	{
		cout << "open file erro!" << endl;
		return false;
	}
}

void group(vector<vector<int>>& j_arr, vector<vector<int>> &ans)
{
	sort(j_arr.begin(), j_arr.end(), [](vector<int>& fir, vector<int>& sec)
		{
			if (fir[1] == sec[1])			//根据t1.id2、 t2.id2排序
				return fir[3] < sec[3];
			else
				return fir[1] < sec[1];
		});
	int size = j_arr.size();
	int max_t1_id1 = j_arr[0][0], min_t2_id1 = j_arr[0][2];
	int i = 0;
	int cnt = 0;
	for (i = 1; i < size; i++)
	{
		if ((j_arr[i - 1][1] == j_arr[i][1]) && (j_arr[i - 1][3] == j_arr[i][3]))
		{
			j_arr[i][0] > max_t1_id1 ? max_t1_id1 = j_arr[i][0] : NULL;
			j_arr[i][2] < min_t2_id1 ? min_t2_id1 = j_arr[i][2] : NULL;
		}
		else
		{
			ans[cnt][0] = max_t1_id1;
			ans[cnt][1] = min_t2_id1;
			ans[cnt][2] = j_arr[i - 1][1];
			ans[cnt][3] = j_arr[i - 1][3];
			max_t1_id1 = j_arr[i][0];
			min_t2_id1 = j_arr[i][2];
			cnt++;
		}
	}
	ans[cnt][0] = max_t1_id1;
	ans[cnt][1] = min_t2_id1;
	ans[cnt][2] = j_arr[i - 1][1];
	ans[cnt][3] = j_arr[i - 1][3];
	cnt++;
	ans.erase(ans.begin() + cnt, ans.end());
	//-------------------------------------------
	sort(ans.begin(), ans.end(), [](vector<int>& fir, vector<int>& sec)
		{
			if (fir[0] == sec[0])
			{
				if (fir[3] == sec[3])
					return fir[2] < sec[2];
				else
					return fir[3] < sec[3];
			}
			else
				return fir[0] < sec[0];
		});
}
