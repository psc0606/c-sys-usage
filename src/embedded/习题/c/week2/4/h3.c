/*----------------------------------------
**	3.3025这个数具有一个独特的性质，将它平分为
**	两段，即30和25，使之相加后求平方，即：
**	(30+25) 2,恰好等于3025本身，请求出具有这
**	样性质的全部四位数。
**----------------------------------------
*/

#include <stdio.h>
void main()
{
	int i, j, k, m, n = 0;
	for (i = 1000;i <=9999; i++)
	{
		j = i % 100;
		k = i / 100;
		m = j + k;
		if ( m * m == i)
		{
			n++;
			printf("(%d+%d)^2 = %d\n",j,k,i);
		}
	}
	printf("num = %d\n",n);
}
