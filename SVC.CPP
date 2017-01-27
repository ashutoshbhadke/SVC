//
// Simple Version Control
//
// Features:
//	* No limit on the line length and the size of the text file
//	* Edit/Delete/Insert anywhere in the file and it will be versioned.
//	* Revert to any previous version of the file.
//	* Only the diff file will be stored as a version. Saves disk space!
//
// Limitations:
//	* File should have a 'txt' extension
//
// Usage:
// svc <filename> [V | version] [rev]
//    filename - A text file to commit
//    V - Specifies the version number of the file to display
//    version - Display the latest version-no and size of the text file
//    rev - Revert the given file to the specified version
// e.g.
//  "svc t1.txt"   -- commit the file
//  "svc t1.txt 2" -- displays the 2nd version of the file
//  "svc t1.txt version -- displays the latest version-no and size of the text file
//  "svc t1.txt 1 rev" -- reverts to the 1st version of the file
//

#include<iostream.h>
#include<string.h>
#include<stdlib.h>
#include<fstream.h>
#include<ctype.h>
#include<stdio.h>

fstream fv,fv1,fp,ff,ft,ftemp1,femp2;

void command();
int getline(fstream &,char []);
void int2str(int ,char []);

int main(int args,char *argv[])
{
    char str[100],ext[10],fpath[120],vpath[120],str1[120],str2[120];
    int i,j,k,len,flag=0,flag1,flag3,flag4,p;
    char ch;

    if(args!=2 && args!=3 && args!=4)
	command();          //shows error message command not properly entered

    len=strlen(argv[1]);

    if(len>4)       //if filename or not
    {
	strcpy(str,argv[1]);

	j=0;
	for(i=len-4;i<len;i++)
	    ext[j++]=str[i];
	ext[j]='\0';

	if(strcmp(ext,".txt")!=0)
	    command();
    }

    else        //not filename
	command();

    fp.open(argv[1],ios::in);   //checking existance of file
    if(!fp)
    {
	cout<<"Error: File does not exist\n";
	return 0;
    }
    fp.close();

    if(args==3 || args==4)     //svc filename version-no command method
    {
	if(args==4)
	{
		if(strcmp(argv[3],"rev")!=0)
			command();
		cout<<"All versions greater than "<<argv[2]<<" will be deleted!\n";
		cout<<"Are you sure to revert the file to version "<<argv[2]<<"? (Y/N)\n";
		cin>>str;

		if(strcmp(str,"Y")!=0 && strcmp(str,"y")!=0)
			exit(0);
	}

	strcpy(str,argv[2]);    //checking version-no validation
	len=strlen(str);
	for(i=0;i<len;i++)
	{
	    if((int)str[i]>57 || (int)str[i]<48)
            {
            	if(strcmp(argv[2],"version")==0)
                {
                	strcpy(str,argv[1]);        //building the version-name path
			str[strlen(str)-4]='\0';    //appending directiry name
			strcat(str,"\\version.txt");

                        fp.open(str,ios::in);

                        if(fp)
                        {
                        	long l;
                        	fp>>ext;
                                fp.close();

				fp.open(argv[1],ios::in | ios::binary);
                                fp.seekg(0,ios::end);
                                l=fp.tellg();
                                l=l/1024;

                                cout<<"Current version: "<<ext<<"\nFilename: "<<argv[1]<<"\nFilesize: "<<l<<" kb\n";
                                fp.close();
                                exit(0);
                        }

                        else
                        {
                        	cout<<"No versions are available\n";
                                exit(0);
                        }
                }

                command();
            }
	}

	strcpy(str,argv[1]);        //building the version-name path
	str[strlen(str)-4]='\0';    //appending directiry name
	strcat(str,"\\version");
	strcpy(vpath,str);	    //vpath- dir_name/version
	strcat(str,argv[2]);        //appendind version-no of file
	strcat(str,".txt");

	fp.open(str,ios::in);               //checking existance of version file

	if(!fp)
	{
	    cout<<"Error: File does not exist\n";
	    return 0;
	}

	if(args==4)
	{
		ff.open(argv[1],ios::out);
		flag3=0;
	}

	i=0;

	while(!fp.eof())       //displaying version file
	{
	    ch=fp.get();
	    fp.get();

	    switch(ch)
	    {
		case '-':
			i++;
			break;

		case '+':
			flag=getline(fp,str);
			if(args==4)
			{
				if(flag3==0)
				{
					ff<<str;
					flag3=1;
				}

				else
					ff<<"\n"<<str;
			}

			else
				cout<<str<<"\n";

			if(flag==0)
			{
				fp.close();
				if(args==4)
					goto z3;
				exit(0);
			}
			//i++;
			break;

		case '*':
			k=atoi(argv[2]);
			while(1)
			{
				k--;
				int2str(k,ext);
				strcpy(str,vpath);
				strcat(str,ext);
				strcat(str,".txt");

				fv.open(str,ios::in);

				j=0;
				while(j<i)
				{
					ch=fv.get();
					if(ch=='-')
					{
						ch=fv.get();
						continue;
					}

					getline(fv,str);
					j++;
				}

				z2:
				ch=fv.get();
				fv.get();

				if(ch=='-')
					goto z2;

				if(ch=='+')
				{
					flag=getline(fv,str);

					if(args==4)
					{
						if(flag3==0)
						{
							ff<<str;
							flag3=1;
						}

						else
							ff<<"\n"<<str;
					}

					else
						cout<<str<<"\n";

					fv.close();
					break;
				}

				else
					fv.close();
			}
			i++;
			break;
	    }
	}

	fp.close();

	if(args==4)
	{
		z3:
		ff.close();
		i=atoi(argv[2]);

		strcpy(str,vpath);
		strcat(str,".txt");

		ff.open(str,ios::in);
		ff>>k;
		ff.close();

		for(i=i+1;i<=k;i++)
		{
			int2str(i,str1);
			strcpy(str,vpath);
			strcat(str,str1);
			strcat(str,".txt");
			remove(str);
		}

		k=atoi(argv[2]);

		strcpy(str,vpath);
		strcat(str,".txt");

		ff.open(str,ios::out);
		ff<<argv[2];
		ff.close();

		strcpy(fpath,vpath);
		fpath[strlen(fpath)-7]='\0';

		goto z;
	}
	return 0;
    }



    //svc filename command method

    strcpy(str,argv[1]);            //building path for version.txt
    str[strlen(str)-4]='\0';        //version.txt- shows newest version no
    strcpy(fpath,str);              //folder path
    strcat(str,"\\version.txt");
    strcpy(vpath,str);

    fp.open(str,ios::in);           //for checking if any version is created or not

    if(!fp)                   //if no version is created
    {
	strcpy(ext,"mkdir ");       //creating version-folder and path required
	strcat(ext,argv[1]);
	ext[strlen(ext)-4]='\0';
	system(ext);

	//strcat(ext,"\version.txt");  //making of version.txt
	fv.open(vpath,ios::out);
	fv<<0;                      //adding initial entry as 0-version no
	fv.close();

	vpath[strlen(vpath)-4]='\0';    //creating version0.txt
	strcat(vpath,"0.txt");

	fp.open(argv[1],ios::in);   		//opening user input file
	fv.open(vpath,ios::out);        //opening version0.txt
	vpath[strlen(vpath)-12]='\0';
	strcat(vpath,"file.txt");
	fv1.open(vpath,ios::out);

	i=1;    			//copying whole to the version0.txt
	while(fp)
	{
	    flag=getline(fp,str);    	//flag=1 - new line

	    fv<<"+ "<<str;
	    fv1<<str;

	    if(flag==1)
	    {
		fv<<"\n";
		fv1<<"\n";
	    }

	    else
		break;
	    i++;
	}

	fp.close();
	fv.close();
	fv1.close();
	cout<<"Commit succesful! Version 0 is created\n";
	exit(0);
    }

    else
    {
	fp>>ext;	//getting the value of latest version of version file
	k=atoi(ext);
	fp.close();
	k++;
    }

    len=strlen(str);
    str[len-4]='\0';
    strcat(str,ext);
    strcat(str,".txt");

    fp.open(argv[1],ios::in);  		//user input file

    //fv.open(str,ios::in);		//old version file

    strcpy(str,fpath);
    strcat(str,"\\file.txt");
    ff.open(str,ios::in);      	//latest user file

    flag=0;
    flag3=0;			//for the condition of the new at the new version file
    flag4=0;			//for checking end of recent file
    i=0;

    int2str(k,str1);
    strcpy(str,fpath);
    strcat(str,"\\version");
    strcat(str,str1);
    strcat(str,".txt");
    ft.open(str,ios::out);	//new verion file

    if(!fp.eof())
    {
	while(flag4==0 && !fp.eof())
	{
		getline(fp,str1);

		if(!ff.eof())
			getline(ff,str2);
		else
		{
			flag4=1;
			break;
		}

		i++;

		z1:

		if(strcmp(str1,str2)==0)
		{
			if(flag3==0)
			{
				ft<<"*";
				flag3=1;
			}
			else
				ft<<"\n*";
		}

		else
		{
			//flag2=getline(ff,str2);

			flag=1;
			j=i+1;
				while(1)
				{
					getline(fp,str1);

					if(strcmp(str1,str2)==0)
					{
						fp.close();

						fp.open(argv[1],ios::in);
						for(p=0;p<i;p++)
						{
							getline(fp,str1);
						}

						for(;p<j;p++)
						{
							if(flag3==1)
								ft<<"\n+ "<<str1;
							else
							{
								ft<<"+ "<<str1;
								flag3=1;
							}

							getline(fp,str1);
							i=j;

						}

						ft<<"\n*";
						break;
					}

					else
						j++;

					if(fp.eof())		//end of user input file
					{
						fp.close();
						fp.open(argv[1],ios::in);
						for(p=0;p<i;p++)
						{
							getline(fp,str1);
						}

						if(flag3==0)
						{
							ft<<"-";
							flag3=1;
						}

						else
							ft<<"\n-";

						if(!ff.eof())
						{
							getline(ff,str2);
							goto z1;
						}

						else
						{
							flag4=1;
							break;
						}
					}

				}
		}
	}

	while(!ff.eof())
	{
		flag=1;
		getline(ff,str2);
		if(flag3==0)
		{
			ft<<"-";
			flag3=1;
		}

		else
			ft<<"\n-";
	}

	if(flag4==1)
	{
		ff.close();
		//fv.close();


		while(fp)
		{
			if(flag3==0)
			{
				ft<<"+ "<<str1;
				flag3=1;
			}

			else
				ft<<"\n+ "<<str1;

			if(fp.eof())
				break;

			getline(fp,str1);
		}

		fv1.close();
		fp.close();

		strcpy(vpath,fpath);
		strcat(vpath,"\\version.txt");
		fp.open(vpath,ios::out);
		fp<<k;
		fp.close();
		goto z;
	}

	fv1.close();
	fp.close();

	if(flag==0)
	{
		remove(str);
		ff.close();
		cout<<"INFO: No new version is created as old and new files are identical\n";
		exit(0);
	}


	else
	{
		strcpy(vpath,fpath);
		strcat(vpath,"\\version.txt");
		fp.open(vpath,ios::out);
		fp<<k;
		fp.close();
	}
	ff.close();

	z:
	strcpy(str,fpath);
	strcat(str,"\\file.txt");

	ff.open(str,ios::out);
	fp.open(argv[1],ios::in);

	while(!fp.eof())
	{
		flag=getline(fp,str);

		ff<<str;

		if(flag==1)
			ff<<"\n";

		else
			break;
	}

	fp.close();
	ff.close();
	cout<<"Commit successful! Version "<<k<<" is created\n";
	exit(0);
    }

    else
	exit(0);
}

void command()
{
    cout<<"Command is not properly entered\n";
    cout<<"\nPlease check for the following syntax\n";
    cout<<"svc <filename> [V | version] [rev]\n\n";
    cout<<"Filename\t A text file to commit\n";
    cout<<"V\tSpecifies the version number of the file to display\n";
    cout<<"version\tDisplay the latest version-no of the text file\n";
    cout<<"rev\tRevert the given file to the specified version\n";
    exit(0);
}

int getline(fstream &f,char s[])
{
    char ch;
    int i=0;
    while(f)
    {
	ch=f.get();
	if(ch=='\n')
	{
	    s[i++]='\0';
	    return 1;
	}

	if(ch==EOF)
	{
	    s[i++]='\0';
	    return 0;
	}
	s[i++]=ch;
    }
}

void int2str(int n,char str[])
{
	int i=0,j;

	if(n==0)
	{
		str[0]='0';
		str[1]='\0';
		return;
	}

	while(n>0)
	{
		j=n%10;
		n=n/10;

		switch(j)
		{
			case 0:
				str[i++]='0';
				break;

			case 1:
				str[i++]='1';
				break;

			case 2:
				str[i++]='2';
				break;

			case 3:
				str[i++]='3';
				break;

			case 4:
				str[i++]='4';
				break;

			case 5:
				str[i++]='5';
				break;

			case 6:
				str[i++]='6';
				break;

			case 7:
				str[i++]='7';
				break;

			case 8:
				str[i++]='8';
				break;

			default:
				str[i++]='9';
		}
	}
	str[i]='\0';
	strrev(str);
}