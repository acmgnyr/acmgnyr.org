/*
 * solve max-sum problem
 * Steve Tate
 */

#define MAXN 150

int mat[MAXN+1][MAXN+1];

#define strip(i,j,h) (mat[i+h][j]-mat[i][j])

main()
{
    int n;
    int i,j,k;
    int msf, gmax;

    scanf("%d",&n);

    for (j=0; j<n; j++)
	mat[0][j] = 0;

    for (i=1; i<=n; i++)
	for (j=0; j<n; j++)
	    scanf("%d",&mat[i][j]);


    for (j=0; j<n; j++)
	for (i=1; i<=n; i++)
	    mat[i][j] = mat[i-1][j] + mat[i][j];


    gmax = mat[1][0];	/* any element of the array */
    for (k=1; k<=n; k++)
	for (i=0; i<=n-k; i++)
	    for (msf=0,j=0; j<n; j++) {
		if (msf >= 0) msf += strip(i,j,k);
		else msf = strip(i,j,k);
	        if (msf > gmax) gmax = msf;
	    }

    printf("%d\n",gmax);
}

