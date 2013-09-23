//**�ӿں���MT_transform**//

#ifndef MT_TRANSFORM
#define MT_TRANSFORM
#include <pthread.h>
#include <algorithm>
#include <vector>
#include <map>
#define INFINITE 0
namespace Husky
{
    typedef int HANDLE;
    typedef int DWORD;

    using namespace std;

    ///////////////**********************************************************************************//////////////
    //��ThreadMananger Ϊһ���򵥵��̰߳�װ������� ��ӿڽ������ÿ���ӿں������� main.cpp�в���������
    //����ӿ�Ϊ�����й��г�Ա��������Bind���������ֿռ�simpleThread��һ�����нӿ�

    //ʹ�÷�����      1��������    ThreadMananger manager;

    //                2�����߳�     ��1�� ��׼�̺߳�������   
    //                              ��2�� �������󷽷�        A ֱ�Ӻ�������
    //                                                        B ��Ӻ������󣨰��̺߳���ָ�룬���̲߳�����
    //                                                        C �޲κ���ָ�뷽����

    //                3�ȴ��߳�     ��1�� �ȴ������߳�        Wait  ����
    //                               (2)  �ȴ�����߳�        WaitMultipleThread

    //                4ThreadMananger�������ٺ��Զ��ر��߳̾��

    //                5���ڴ������̺߳������ؾ������Ϊ����������ø�����������������Ҫ��API����CloseHandle�رգ�

    //                6���ڴ����̵߳Ĳ�������Ǽ��������� ������ָ�룬�򵥵Ľṹ��ȣ�����������ڿ�������Ӱ��Ч�ʣ�

    //                7������Դй¶�����û���ͣ�Ĵ����µ��̣߳������̷߳������˳������ڴ�й¶--��̬������̲߳���й¶��
    ///////////////**********************************************************************************//////////////


    namespace simpleThread
    {
        typedef void *(* PThreadFunc)(void* param);



        //�Ժ�������ʽ�����߳�ʱ���ڲ����õ��̺߳��������ýṹ���װ�Դ���FuncObj������Ϣ
        //��ʹ�õĺ�����������һ��Generator �������κβ����Ϳ��Ե��õĺ������� 
        //�̲߳����ɺ�������ĳ�Ա����Я��,�ù��캯����ʼ���ó�Ա������
        template <class FuncObj>
            struct InnerThreadFuncCls//**�ýṹ��װ�Դ���������Ϣ
            {
                static void *InnerThreadFunc(void *pPara)
                {
                    FuncObj* pFuncObj=(FuncObj*)pPara;
                    (*pFuncObj)();	
                    delete pFuncObj;//����̬��������			
                    return 0;
                }
            };



        class threadManager                       //�̹߳����ࣻ
        {
            public:
                threadManager(){;}
                ~threadManager()
                {
                    //�ر�������ÿһ���߳̾��������Щ�߳̿����л������еģ���
                }


                //�����������߳�����
                unsigned int HandleCount(){return m_vecHandle.size();}


                //�ر�������ÿһ���߳̾���������ڲ���Դ������Щ�߳̿����л������еģ���
                void clear()
                {
                    //WaitMultipleThread();
                    m_vecHandle.clear();
                    //	m_vecPvoid.clear();
                }




                //��_beginthreadex�̺߳�����ʽ�����߳�
                //pFuncָ���׼�̺߳�����ָ��  unsigned __stdcall ThreadFunc(void *param )
                //pPara�̲߳���ָ��           : �˲���ָ��Ķ����������賤���̺߳���������ע��Ϊ����߳�����ǡ���Ĳ�������
                //return value                : ����ֵΪ��˵�������߳�ʧ�ܣ����򷵻��߳̾��
                HANDLE CreateThread( PThreadFunc pFunc,void *pPara)
                {	
                    pthread_t pt;
                    int nErrorCode=pthread_create(&pt,NULL,pFunc,pPara);
                    if(nErrorCode!=0)
                      return nErrorCode;
                    m_vecHandle.push_back(pt);	//�����߳��б� ΪWaitForMultipleObjects׼��	
                    return nErrorCode;

                }

                //�ȴ�ĳ���߳�ִ����ϣ�
                //hThread�߳̾��         : Ϊ0ʱΪĬ�����һ��������������߳̾�� 
                //dwMilliseconds�ȴ�ʱ��  : ��λ���룬Ĭ��ֵ����ʱ��
                //return value            : -1�����Ч������ֵ WaitForSingleObject�����ķ���ֵ
                DWORD Wait(HANDLE hThread=0,DWORD dwMilliseconds=INFINITE )
                {
                    if( hThread==0)//���һ��������߳�
                    {   
                        if(!m_vecHandle.empty())
                        {
                            return pthread_join(m_vecHandle.back(),NULL);
                        }
                        else
                          return -1;
                    }
                    else
                    {
                        if (find(m_vecHandle.begin(),m_vecHandle.end(),hThread)==m_vecHandle.end())//�����ڴ˾��
                        {
                            return -1;
                        }

                        return pthread_join(hThread, NULL);
                    }

                }


                //�ȴ������߳�ִ�����
                //bWaitAll�Ƿ������߳�  : Ĭ��ֵ1�ȴ������߳�,0���κ��߳̽������˺�������
                //dwMilliseconds        : ��λ���룬Ĭ��ֵ����ʱ��
                //return value          : -1û���κξ��������ֵ WaitForMultipleObjects�����ķ���ֵ
                DWORD  WaitMultipleThread( bool bWaitAll=1,DWORD dwMilliseconds=INFINITE)
                {
                    if (m_vecHandle.empty())		
                      return -1;	
                    int nErrorcode;
                    for (uint i=0;i<m_vecHandle.size();++i)
                    {
                        nErrorcode=pthread_join(m_vecHandle[i], NULL); 
                        if (nErrorcode!=0)
                          return nErrorcode;	
                    }	
                    return 0;
                }



                //�Ժ�������ʽ�����߳�,�����޲εĺ���ָ�룻void (*pfunc)(void)
                //��ʹ�õĺ�����������һ��Generator �������κβ����Ϳ��Ե��õĺ������� 
                //�̲߳����ɺ�������ĳ�Ա����Я��,�ù��캯����ʼ���ó�Ա������
                template <class FuncObj>
                    HANDLE CreateThread(FuncObj funcObj) //���ô�����ָ����ָ��ʱ ���б������������ֵ����
                    {
                        FuncObj* pFuncObj=new FuncObj(funcObj);//��Ҫ���ʵ�Ĭ�Ͽ������캯��
                        //���̺߳�����DELETE
                        HANDLE handle;
                        pthread_t pt;
                        int nErrorCode=pthread_create(&pt,NULL,InnerThreadFuncCls<FuncObj>::InnerThreadFunc,pFuncObj);
                        if(nErrorCode!=0)
                          return nErrorCode;
                        m_vecHandle.push_back(pt);	//�����߳��б� ΪWaitForMultipleObjects׼��	
                        return nErrorCode;
                    }
            private:

                vector<pthread_t> m_vecHandle;   //�߳̾���ͺ���������б�

                //	vector<void*> m_vecPvoid;  //�����̲߳���ʹ�ã�
            private:
                threadManager(const threadManager&){;}//��ֹ����
                void operator=(const threadManager &){}//��ֹ��ֵ			
        };





        //�������������ڰ��̺߳���ָ����̲߳���
        template<class PUserFunc,class ThreadPara>
            struct FuncObjProxy 
            {		
                FuncObjProxy(PUserFunc pFunc,ThreadPara para):m_pFunc(pFunc),m_para(para)
                {;	}
                void operator()()
                {
                    m_pFunc(m_para);

                }
                PUserFunc m_pFunc;
                ThreadPara m_para;

            };

        //�󶨷��������غ�������
        //pfun ����ָ��ָ���û��̺߳�������ʽΪ typename RetCls (*ThreadFunc)(typename ThreadPara) 
        //ʵ�ʷ�������RetCls�û���ȡ���������Ϊvoid (*ThreadFunc)(ThreadPara para)��ʽ
        //refParaΪ�û��̺߳�������������
        template<class PUserFunc,class ThreadPara>
            FuncObjProxy<PUserFunc,ThreadPara> Bind(PUserFunc pfun,ThreadPara &refPara)
            {
                return FuncObjProxy<PUserFunc,ThreadPara>(pfun,refPara);
            }


    }



}



#endif
