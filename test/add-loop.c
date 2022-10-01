
int main()
{
    long long bill, i;
    bill=-1000000;

    for (i=0; i<9073741826; i++)
	bill=i+bill;
    
    return 0;
}
