#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

template <typename T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
    for (const auto& t : v)
        os << t << '\n';
    return os;

}

ostream& operator<<(ostream& os, const vector<bool>& v)
{
    for (bool b : v)
        os << (b ? '#' : '|');
    return os;
}

ostream& operator<<(ostream& os, const vector<char>& v)
{
    for (char c : v)
        os << c;
    return os;
}

class Stack{

    vector< vector<char> > levels;
    int y=0;
public:
    Stack(int height) : levels(height, vector<char>(6, '.'))
    {
    }

    bool is_full()
    {
        return y == (int)(levels.size());
    }

    int get_level()
    {
        return y;
    }

    bool does_fit(const vector<bool>& disc)
    {
        for (int i=0;i<(int)(disc.size());i++)
            if (levels[y][i] == '|' && disc[i] == true)
                return false;
        return true;
    }

    bool push(const vector<bool>& disc)
    {
        bool valid = true;
        for (int i=0;i<(int)(disc.size());i++)
        {
            if (disc[i])
                levels[y][i] = '#';
            else
            {
                if (levels[y][i] == '.')
                {
                    levels[y][i] = 'T';
                    int up3 = y+3;
                    if (up3 > (int)(levels.size()))
                    {
                        valid = false;
                        up3 = (int)(levels.size());
                    }
                    for (int a=y+1; a<up3; a++)
                        levels[a][i] = '|';
                }
            }
        }
        y+=1;
        return valid;
    }

    void pop(const vector<bool>& disc)
    {
        y-=1;
        for (int i=0;i<(int)(disc.size());i++)
        {
            if (disc[i])
                levels[y][i] = '.';
            else
            {
                if (levels[y][i] == 'T')
                {
                    int up3 = y+3;
                    for (int a=y; a<up3; a++)
                        levels[a][i] = '.';
                }   
            }
        }
    }

    friend ostream& operator<<(ostream& os, const Stack& s);
};

ostream& operator<<(ostream& os, const Stack& s)
{
    os<< s.levels;
    return os;
}

vector<bool> reverse(const vector<bool>& disc)
{
    vector<bool> rev(disc.rbegin(), disc.rend());
    return rev;
}

vector< vector<bool> > all_rotations(const vector<bool>& disc)
{
    vector< vector<bool> > rotations;
    rotations.push_back(disc);
    int disc_size = disc.size();
    for (int i=1;i<(int)(disc.size());i++)
    {
        vector<bool> v;
        v.reserve(disc_size);
        for (int x=i; x<i+disc_size; x++)
        {
            v.push_back( disc[x % disc_size] );
        }
        rotations.push_back(v);
    }
    return rotations;
}

vector< vector<bool> > all_options(const vector<bool>& disc)
{
    vector< vector<bool> > options1 = all_rotations(disc);
    vector<bool> rev = reverse(disc);
    vector< vector<bool> > options2 = all_rotations(rev);
    options1.insert(options1.end(), options2.begin(), options2.end());
    std::sort(options1.begin(), options1.end());
    auto last = std::unique(options1.begin(), options1.end());
    options1.erase(last, options1.end());
    return options1;
}

void solve_recursive(Stack& stack, 
                    const vector< vector<bool> >& discs,
                    const vector< vector< vector<bool>>>& disc_options, 
                    vector<bool>& disc_used);

void try_disc_recursive(Stack& stack, 
                    const vector<bool>& disc,
                    const vector< vector<bool> >& discs,
                    const vector< vector< vector<bool>>>& disc_options, 
                    vector<bool>& disc_used)
{
    //cout<<"disc:"<<disc<<'\n';
    if (stack.does_fit(disc))
    {
        bool valid = stack.push(disc);
        //cout<<stack<<'\n';
        
        if (valid)
            solve_recursive(stack, discs, disc_options, disc_used);
        stack.pop(disc);
        
    }
}

void solve_recursive(Stack& stack, 
                    const vector< vector<bool> >& discs,
                    const vector< vector< vector<bool>>>& disc_options, 
                    vector<bool>& disc_used)
{
    if (stack.is_full())
    {
        cout<<"solution:"<<'\n'<<stack<<'\n';
        return;
    }
    for (int i=0;i<(int)(disc_options.size());i++)
    {
        if (!disc_used[i])
        {   
            disc_used[i] = true;
            if (stack.get_level() == 0)
            {
                try_disc_recursive(stack,
                                    discs[i],
                                    discs,
                                    disc_options,
                                    disc_used);
            }
            else
            {
                for (vector<bool> disc : disc_options[i])
                {
                    try_disc_recursive(stack,
                                        disc,
                                        discs,
                                        disc_options,
                                        disc_used);
                }
            }
            disc_used[i] = false;
        }
    }
}

void solve(Stack& stack, const vector< vector<bool> >& discs)
{
    vector< vector< vector<bool>>> disc_options; 
    for (auto disc : discs)
        disc_options.push_back(all_options(disc));
    vector< bool> disc_used(discs.size(), false);
    /*for (vector< vector<bool>> options : disc_options)
    {
        cout<<"===================="<<'\n';
        cout<<options<<'\n';
    }
    */
    solve_recursive(stack, discs, disc_options, disc_used);
}

int main()
{
    vector< vector<bool> > discs= { 
        {0,1,0,1,1,1},
        {0,1,1,0,1,1},
        {0,0,0,1,0,1},
        {0,0,0,0,1,1},
        {0,0,0,0,0,0},
        {0,1,0,1,0,1},
        {0,0,0,1,1,1},
        {0,1,1,1,1,1},
        {0,0,1,0,1,1},
        {0,0,1,1,1,1},
        {0,0,0,0,0,1},
        {0,0,1,0,0,1},
        };
    //cout<< discs << '\n';
    Stack stack((int)(discs.size()));
    //cout<<stack<<'\n';
    solve(stack, discs);
}