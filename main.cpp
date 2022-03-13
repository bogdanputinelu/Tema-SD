#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <chrono>
#include <algorithm>
#include <fstream>
using namespace std;
using namespace std::chrono;
bool test(const vector<int> &v,const int &n){
    int i=1;
    while(i<n){
        if(v[i-1]>v[i])
            return false;
        ++i;
    }
    return true;
}
void countsort(vector<int> &v, const int &n){
    auto start=high_resolution_clock::now();
    int max=v[0],i;
    vector<int> fr;
    for(i=1;i<n;++i){
        if(max<v[i])
            max=v[i];
    }
    for(i=0;i<=max;++i)
        fr.push_back(0);
    for(i=0;i<n;++i)
        fr[v[i]]++;
    i=-1;
    for(int j=0;j<=max;++j){
        while(fr[j]>0){
            v[++i]=j;
            fr[j]--;
        }
    }
    auto stop=high_resolution_clock::now();
    auto duration= duration_cast<microseconds>(stop-start);
    if(test(v,v.size()))
        cout<<"Countsort a sortat in:"<<duration.count()/1000000.00000000000000<<"seconds"<<endl<<endl;
    else
        cout<<"Countsort nu a sortat bine"<<endl<<endl;
}
void shellsort(vector<int> &v, const int &n){
    auto start=high_resolution_clock::now();
    for(int i=n/2;i>0;i/=2){
        for(int j=i;j<n;++j){
            int k,aux=v[j];
            for(k=j;k>=i && v[k-i]>aux;k-=i){
                v[k]=v[k-i];
            }
            v[k]=aux;
        }
    }
    auto stop=high_resolution_clock::now();
    auto duration= duration_cast<microseconds>(stop-start);
    if(test(v,v.size()))
        cout<<"Shellsort a sortat in:"<<duration.count()/1000000.00000000000000<<"seconds"<<endl<<endl;
    else
        cout<<"Shellsort nu a sortat bine"<<endl<<endl;
}
void radixsort10(vector<int> &v,const int &n){
    auto start=high_resolution_clock::now();
    int max=v[0],i,bucket[10],cif=1;
    vector<int> aux;
    aux.reserve(n);
    for(i=1;i< n;i++)
        if(max<v[i])
            max=v[i];
    while(max){
        for(i=0;i<10;++i)
            bucket[i]=0;
        for(i=0;i<n;++i){
            bucket[(v[i]/cif)%10]++;
        }
        for(i=1;i<10;++i){
            bucket[i]+=bucket[i-1];
        }
        for(i=n-1;i>=0;--i){
            aux[bucket[(v[i]/cif)%10]-1]=v[i];
            bucket[(v[i]/cif)%10]--;
        }
        for(i=0;i<n;++i)
            v[i]=aux[i];
        cif*=10;
        max/=10;
    }
    auto stop=high_resolution_clock::now();
    auto duration= duration_cast<microseconds>(stop-start);
    if(test(v,v.size()))
        cout<<"Radixsort (10) a sortat in:"<<duration.count()/1000000.00000000000000<<"seconds"<<endl<<endl;
    else
        cout<<"Radixsort (10) nu a sortat bine"<<endl<<endl;
}
void radixsort(vector<int> &v,const int &n){
    auto start=high_resolution_clock::now();
    int max=v[0],i,bucket[65536],p=0;
    vector<int> aux;
    aux.reserve(n);
    for(i=1;i< n;i++)
        if(max<v[i])
            max=v[i];
    while(max){
        for(i=0;i<65536;++i)
            bucket[i]=0;
        for(i=0;i<n;++i){
            bucket[(v[i]>>p)&0xFFFF]++;
        }
        for(i=1;i<65536;++i){
            bucket[i]+=bucket[i-1];
        }
        for(i=n-1;i>=0;--i){
            aux[bucket[(v[i]>>p)&0xFFFF]-1]=v[i];
            bucket[(v[i]>>p)&0xFFFF]--;
        }
        for(i=0;i<n;++i)
            v[i]=aux[i];
        p+=16;
        max/=65536;
    }
    auto stop=high_resolution_clock::now();
    auto duration= duration_cast<microseconds>(stop-start);
    if(test(v,v.size()))
        cout<<"Radixsort (2^16) a sortat in:"<<duration.count()/1000000.00000000000000<<"seconds"<<endl<<endl;
    else
        cout<<"Radixsort (2^16) nu a sortat bine"<<endl<<endl;
}
int partition(vector<int> &v,int first, int last){
    int pivot=v[last];
    int i=first-1;
    for (int j = first; j <= last - 1; ++j) {
        if(v[j]<pivot){
            i++;
            swap(v[i],v[j]);
        }
    }
    swap(v[i+1],v[last]);
    return i+1;
}
void quicksortlast(vector<int> &v,int first,int last){
    if(first<last){
        int pivot= partition(v,first,last);
        quicksortlast(v,first,pivot-1);
        quicksortlast(v,pivot+1,last);
    }
}
int mediana3(vector<int> &v,int first,int last){
    int a=v[first],b=v[(first+last)/2],c=v[last-1];
    int i=first-1,j=last;
    if(b>a && b<c || b>c && b<a)
        a=b;
    else
    if(c>a && c<b || c>b && c<a)
        a=c;
    while(true){
        do{
            j--;
        }while(a<v[j]);
        do{
            i++;
        }while(a>v[i]);
        if(i<j)
            swap(v[i],v[j]);
        else
            return j+1;
    }
}
void quicksortmed3(vector<int> &v,int first,int last){
    if(last-first>=2){
        int pivot= mediana3(v,first,last);
        quicksortmed3(v,first,pivot);
        quicksortmed3(v,pivot,last);
    }
}
void merge(vector<int> &v,int st,int mid,int dr){
    vector<int> aux;
    aux.reserve(dr-st+1);
    int i=st,j=mid+1,k=0;
    while(i<=mid && j<=dr){
        if(v[i]>v[j])
            aux[k++]=v[j++];
        else
            aux[k++]=v[i++];
    }
    while(i<=mid)
        aux[k++]=v[i++];
    while(j<=dr)
        aux[k++]=v[j++];
    for(i=st;i<=dr;i++)
        v[i]=aux[i-st];
}
void mergesort(vector<int> &v,int st,int dr){
    if(st<dr){
        int mid=(st+dr)/2;
        mergesort(v,st,mid);
        mergesort(v,mid+1,dr);
        merge(v,st,mid,dr);
    }
}
int main() {
    ifstream f("D:\\un alt proiect c++\\input.in");
    //ifstream f("input.in");
    int teste,max,elemente;
    if(f.is_open()) {
        vector<int> v, vect;
        int j;
        srand(time(NULL));
        f >> teste;
        for (int i = 0; i < teste; ++i) {
            f >> elemente >> max;
            cout << "Testul " << i + 1 << " cu " << elemente << " numere de valoare maxima " << max << endl << endl;
            for (j = 0; j < elemente; ++j) {
                v.push_back(rand() * rand() % max);
            }

            for (j = 0; j < elemente; ++j) {
                vect.push_back(v[j]);
            }
            auto start = high_resolution_clock::now();
            mergesort(vect, 0, vect.size() - 1);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            if (test(vect, vect.size()))
                cout << "Mergesort a sortat in:" << duration.count() / 1000000.00000000000000 << "seconds" << endl
                     << endl;
            else
                cout << "Mergesort nu a sortat bine" << endl << endl;

            vect.clear();
            for (j = 0; j < elemente; ++j) {
                vect.push_back(v[j]);
            }
            auto start1 = high_resolution_clock::now();
            quicksortlast(vect, 0, vect.size() - 1);
            auto stop1 = high_resolution_clock::now();
            auto duration1 = duration_cast<microseconds>(stop1 - start1);
            if (test(vect, vect.size()))
                cout << "Quicksort cu pivot ultimul element a sortat in:" << duration1.count() / 1000000.00000000000000
                     << "seconds" << endl << endl;
            else
                cout << "Quicksort cu pivot ultimul element nu a sortat bine" << endl << endl;

            vect.clear();
            for (j = 0; j < elemente; ++j) {
                vect.push_back(v[j]);
            }
            auto start2 = high_resolution_clock::now();
            quicksortmed3(vect, 0, vect.size());
            auto stop2 = high_resolution_clock::now();
            auto duration2 = duration_cast<microseconds>(stop2 - start2);
            if (test(vect, vect.size()))
                cout << "Quicksort cu mediana din 3 a sortat in:" << duration2.count() / 1000000.00000000000000
                     << "seconds" << endl << endl;
            else
                cout << "Quicksort cu mediana din 3 nu a sortat bine" << endl << endl;

            vect.clear();
            for (j = 0; j < elemente; ++j) {
                vect.push_back(v[j]);
            }
            auto start3 = high_resolution_clock::now();
            sort(vect.begin(), vect.end());
            auto stop3 = high_resolution_clock::now();
            auto duration3 = duration_cast<microseconds>(stop3 - start3);
            if (test(vect, vect.size()))
                cout << "Nativesort a sortat in:" << duration3.count() / 1000000.00000000000000 << "seconds" << endl
                     << endl;
            else
                cout << "Nativesort nu a sortat bine" << endl << endl;

            vect.clear();
            for (j = 0; j < elemente; ++j) {
                vect.push_back(v[j]);
            }
            radixsort(vect, vect.size());

            vect.clear();
            for (j = 0; j < elemente; ++j) {
                vect.push_back(v[j]);
            }
            radixsort10(vect, vect.size());

            vect.clear();
            for (j = 0; j < elemente; ++j) {
                vect.push_back(v[j]);
            }
            shellsort(vect, vect.size());

            vect.clear();
            for (j = 0; j < elemente; ++j) {
                vect.push_back(v[j]);
            }
            countsort(vect, vect.size());

            vect.clear();
            v.clear();
        }
        f.close();
    }
    else
        cout<<"Nu s-a putut deschide fisierul de intrare";

    return 0;
}
