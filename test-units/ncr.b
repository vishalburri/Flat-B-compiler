declblock{
int n,r,i,j,testcase;
int a,b,result;
}
codeblock{
	read testcase;
	while(testcase!=0)
	{
	read n,r;
	i=1;
	j=1;
L1: 	j=j*i;
	i=i+1;
	goto L1 if(i<=n-r);
	a=1;
	for i=1,n+1{
	a=a*i;
	}
	b=1;
	i=1;
	while(i<=r){
	b=b*i;
	i=i+1;
	}
	result = a/(b*j);
	print result;
	testcase = testcase - 1;
	}
	
}
