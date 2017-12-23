 declblock {
		int a,b,t,n;
		int l1,l2,lcm;
}

codeblock {
	read n;
	while(n!=0){
	read a,b;
	l1=a;
	l2=b;
L1:     t=a;
	a=b;
	b =t%b; 
	goto L1 if(b!=0);
	println "GCD: ",a;
	n=n-1;
	lcm = (l1*l2)/a;
	println "LCM: ",lcm;
	}
	
}




