 declblock {
		int i,j;
		int b[100];
		int c[100];
		int n,x;
		int  t;
}

codeblock {
	read x;
	while(x!=0){
	read n;
	for i=0,n,1{
		 read b[i];
	}
	for i=0,n,1{
		for j=0,n-i-1,1{
			if(b[j]>b[j+1]){
				t=b[j];
				b[j]=b[j+1];
				b[j+1]=t;	
			}
		}
	}
	for i=0,n,1{
		print b[i];
	}
	x= x-1;
	}
	
}




