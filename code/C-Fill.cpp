#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

const int method[][2] = {{0, 1}, {0, 2}, {1, 0}, {1, 2}, {2, 0}, {2, 1}};

struct Result
{
    int d;
    int poured;
};

struct Tree
{
    vector<int> status;
    int poured;
};

int exchange(vector<int> &status, int *div, int first, int second)
{
    const int firstInit = status[first];
    const int secondInit = status[second];
    if (firstInit >= div[second] - secondInit)
    {
        status[first] -= (div[second] - secondInit);
        status[second] = div[second];
        return div[second] - secondInit;
    }
    else
    {
        status[first] = 0;
        status[second] += firstInit;
        return firstInit;
    }
}

Result find(vector<int> status, int *div, int D)
{
    vector<Tree> newRecord{{.status = status, .poured = 0}};
    vector<vector<vector<bool> > > repeat(div[0] + 1, vector< vector<bool> >(div[1] + 1 , vector<bool>(div[2] + 1,false)));
    repeat[0][0][div[2]] = true;
    vector<Result> resultList;
    Result cloestD = {.d = div[2], .poured = 0};

    for (int step = 0; true; step++)
    {
        if (newRecord.size() == 0)
            break;
        vector<Tree> record(newRecord);
        newRecord.clear();
        for (int i = 0; i < record.size(); i++)
        {
            /*cout << '(' << record[i].status[0] << ' ' << record[i].status[1] << ' ' << record[i].status[2] << ' ' << record[i].poured << ")\n";*/
            for (int j = 0; j < 3; j++)
                if (record[i].status[j] == D)
                {
                    resultList.push_back({.d = D, .poured = record[i].poured});
                    continue;
                }
                else if (record[i].status[j] < D)
                    if (D - record[i].status[j] < (D - cloestD.d)||cloestD.d>D)
                        cloestD = {.d = record[i].status[j], .poured = record[i].poured};
                    else if ((D - record[i].status[j] == (D - cloestD.d)||cloestD.d>D) && record[i].poured<cloestD.poured)
                        cloestD = {.d = record[i].status[j], .poured = record[i].poured};

            for (int j = 0; j < 6; j++)
            {
                vector<int> tmp(record[i].status);
                const int detla = exchange(tmp, div, method[j][0], method[j][1]);
                if (!repeat[tmp[0]][tmp[1]][tmp[2]])
                {
                    repeat[tmp[0]][tmp[1]][tmp[2]] = true;
                    newRecord.push_back({.status = tmp, .poured = record[i].poured + detla});
                }
            }
        }
    }

    if (resultList.size())
    {
        int min = resultList[0].poured;
        int pos = 0;
        for (int i = 0; i < resultList.size(); i++)
            if (resultList[i].poured < min)
            {
                pos = i;
                min = resultList[i].poured;
            }
        return resultList[pos];
    }

    return cloestD;
}

int main()
{
    int cases;
    cin >> cases;
    for (int caze = 0; caze < cases; caze++)
    {
        int div[3], D;
        cin >> div[0] >> div[1] >> div[2] >> D;

        vector<int> status{0, 0, div[2]};
        const Result answer = find(status, div, D);
        cout << answer.poured << ' ' << answer.d << '\n';
    }

    return 0;
}