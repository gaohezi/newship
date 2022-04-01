//#include <stdio.h>
//#include <string.h>

//#include <stdlib.h>

// 
//#include "json.h"
// 
//typedef struct
//{
//	int id;
//	char firstName[32];
//	char lastName[32];
//	char email[64];
//	int age;
//	float height;
//}people;
// 
//void dofile(char *filename);/* Read a file, parse, render back, etc. */
// 
//int main(int argc, char **argv)
//{
// 
////	dofile("json_str1.txt");
////	dofile("json_str2.txt");
//	dofile("json_str3.txt");
// 
//	return 0;
//}
// 
////parse a key-value pair
//int cJSON_to_str(char *json_string, char *str_val)
//{
//	cJSON *root=cJSON_Parse(json_string);
//	if (!root)
//	{
//		printf("Error before: [%s]\n",cJSON_GetErrorPtr());
//		return -1;
//	}
//	else
//	{
//		cJSON *item=cJSON_GetObjectItem(root,"firstName");
//		if(item!=NULL)
//		{
//			printf("cJSON_GetObjectItem: type=%d, key is %s, value is %s\n",item->type,item->string,item->valuestring);
//			memcpy(str_val,item->valuestring,strlen(item->valuestring));
//		}
//		cJSON_Delete(root);
//	}
//	return 0;
//}
// 
////parse a object to struct
//int cJSON_to_struct(char *json_string, people *person)
//{
//	cJSON *item;
//	cJSON *root=cJSON_Parse(json_string);
//	if (!root)
//	{
//		printf("Error before: [%s]\n",cJSON_GetErrorPtr());
//		return -1;
//	}
//	else
//	{
//		cJSON *object=cJSON_GetObjectItem(root,"person");
//		if(object==NULL)
//		{
//			printf("Error before: [%s]\n",cJSON_GetErrorPtr());
//			cJSON_Delete(root);
//			return -1;
//		}
//		printf("cJSON_GetObjectItem: type=%d, key is %s, value is %s\n",object->type,object->string,object->valuestring);
// 
//		if(object!=NULL)
//		{
//			item=cJSON_GetObjectItem(object,"firstName");
//			if(item!=NULL)
//			{
//				printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%s\n",item->type,item->string,item->valuestring);
//				memcpy(person->firstName,item->valuestring,strlen(item->valuestring));
//			}
// 
//			item=cJSON_GetObjectItem(object,"lastName");
//			if(item!=NULL)
//			{
//				printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%s\n",item->type,item->string,item->valuestring);
//				memcpy(person->lastName,item->valuestring,strlen(item->valuestring));
//			}
// 
//			item=cJSON_GetObjectItem(object,"email");
//			if(item!=NULL)
//			{
//				printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%s\n",item->type,item->string,item->valuestring);
//				memcpy(person->email,item->valuestring,strlen(item->valuestring));
//			}
// 
//			item=cJSON_GetObjectItem(object,"age");
//			if(item!=NULL)
//			{
//				printf("cJSON_GetObjectItem: type=%d, string is %s, valueint=%d\n",item->type,item->string,item->valueint);
//				person->age=item->valueint;
//			}
//			else
//			{
//				printf("cJSON_GetObjectItem: get age failed\n");
//			}
// 
//			item=cJSON_GetObjectItem(object,"height");
//			if(item!=NULL)
//			{
//				printf("cJSON_GetObjectItem: type=%d, string is %s, valuedouble=%f\n",item->type,item->string,item->valuedouble);
//				person->height=item->valuedouble;
//			}
//		}
// 
//		cJSON_Delete(root);
//	}
//	return 0;
//}
// 
////parse a struct array
//int cJSON_to_struct_array(char *text, people worker[])
//{
//	cJSON *json,*arrayItem,*item,*object;
//	int i;
// 
//	json=cJSON_Parse(text);
//	if (!json)
//	{
//		printf("Error before: [%s]\n",cJSON_GetErrorPtr());
//	}
//	else
//	{
//		arrayItem=cJSON_GetObjectItem(json,"people");
//		if(arrayItem!=NULL)
//		{
//			int size=cJSON_GetArraySize(arrayItem);
//			printf("cJSON_GetArraySize: size=%d\n",size);
// 
//			for(i=0;i<size;i++)
//			{
//				printf("i=%d\n",i);
//				object=cJSON_GetArrayItem(arrayItem,i);
// 
//				item=cJSON_GetObjectItem(object,"firstName");
//				if(item!=NULL)
//				{
//					printf("cJSON_GetObjectItem: type=%d, string is %s\n",item->type,item->string);
//					memcpy(worker[i].firstName,item->valuestring,strlen(item->valuestring));
//				}
// 
//				item=cJSON_GetObjectItem(object,"lastName");
//				if(item!=NULL)
//				{
//					printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%s\n",item->type,item->string,item->valuestring);
//					memcpy(worker[i].lastName,item->valuestring,strlen(item->valuestring));
//				}
// 
//				item=cJSON_GetObjectItem(object,"email");
//				if(item!=NULL)
//				{
//					printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%s\n",item->type,item->string,item->valuestring);
//					memcpy(worker[i].email,item->valuestring,strlen(item->valuestring));
//				}
// 
//				item=cJSON_GetObjectItem(object,"age");
//				if(item!=NULL)
//				{
//					printf("cJSON_GetObjectItem: type=%d, string is %s, valueint=%d\n",item->type,item->string,item->valueint);
//					worker[i].age=item->valueint;
//				}
//				else
//				{
//					printf("cJSON_GetObjectItem: get age failed\n");
//				}
// 
//				item=cJSON_GetObjectItem(object,"height");
//				if(item!=NULL)
//				{
//					printf("cJSON_GetObjectItem: type=%d, string is %s, value=%f\n",item->type,item->string,item->valuedouble);
//					worker[i].height=item->valuedouble;
//				}
//			}
//		}
// 
//		for(i=0;i<3;i++)
//		{
//			printf("i=%d, firstName=%s,lastName=%s,email=%s,age=%d,height=%f\n",
//					i,
//					worker[i].firstName,
//					worker[i].lastName,
//					worker[i].email,
//					worker[i].age,
//					worker[i].height);
//		}
// 
//		cJSON_Delete(json);
//	}
//	return 0;
//}
// 
//// Read a file, parse, render back, etc.
//void dofile(char *filename)
//{
//	FILE *f;
//	int len;
//	char *data;
//	
//	f=fopen(filename,"rb");
//	fseek(f,0,SEEK_END);
//	len=ftell(f);
//	fseek(f,0,SEEK_SET);
//	data=(char*)malloc(len+1);
//	fread(data,1,len,f);
//	fclose(f);
//	
//	printf("read file %s complete, len=%d.\n",filename,len);
// 
////	char str_name[40];
////	int ret = cJSON_to_str(data, str_name);
// 
////	people person;
////	int ret = cJSON_to_struct(data, &person);
// 
//	people worker[3]={{0}};
//	cJSON_to_struct_array(data, worker);
// 
//	free(data);
//}
// 
// 