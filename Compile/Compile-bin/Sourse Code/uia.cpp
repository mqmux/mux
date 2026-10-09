#include <Windows.h>
#include <UIAutomation.h>
#include <stdio.h>

#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "UIAutomationCore.lib")

IUIAutomation* g_pIUAutomation = NULL;

void show_pRoot(HRESULT hr, IUIAutomationElement* pRoot, IUIAutomationCondition* pCondition) {
    IUIAutomationElementArray* pChildren = NULL;
    // 查找根元素下所有子元素（整个子树）
    hr = pRoot->FindAll(TreeScope_Descendants, pCondition, &pChildren);
    if (FAILED(hr) || pChildren == NULL)
    {
        printf("Failed to find child elements.\n");
        pRoot->Release(); // 释放根元素对象
        return;
    }

    int count;
    pChildren->get_Length(&count);
    printf("Number of elements found: %d\n", count);

    // 遍历所有找到的控件元素
    for (int i = 0; i < count; ++i)
    {
        IUIAutomationElement* pElement = NULL;
        pChildren->GetElement(i, &pElement);

        if (pElement)
        {
            BSTR bstrName;
            pElement->get_CurrentName(&bstrName);
            printf("Element %d Name: %S ", i, bstrName);
            SysFreeString(bstrName);

            int controlType;
            pElement->get_CurrentControlType(&controlType);
            printf("Control Type: %d ", controlType);

            RECT boundingRect;
            hr = pElement->get_CurrentBoundingRectangle(&boundingRect);
            if (SUCCEEDED(hr)) printf("%d,%d,%d,%d\n", boundingRect.left, boundingRect.top, boundingRect.right, boundingRect.bottom);

            pElement->Release(); // 释放当前元素对象
        }
    }
    // 释放资源
    if (pChildren)
        pChildren->Release();
}

WCHAR* ConvertCharToWCHAR(char* input)
{
    if (input == nullptr)
        return nullptr;

    int length = MultiByteToWideChar(CP_ACP, 0, input, -1, nullptr, 0);
    if (length == 0)
    {
        return nullptr;
    }

    WCHAR* output = new WCHAR[length];
    if (!MultiByteToWideChar(CP_ACP, 0, input, -1, output, length))
    {
        delete[] output;
        return nullptr;
    }

    return output;
}

// 模拟鼠标点击函数
void SimulateMouseClick(int x, int y)
{
    SetCursorPos(x, y); // 移动鼠标到指定位置
    mouse_event(MOUSEEVENTF_LEFTDOWN, x, y, 0, 0); // 模拟鼠标左键按下
    mouse_event(MOUSEEVENTF_LEFTUP, x, y, 0, 0);   // 模拟鼠标左键释放
}

int main(int argc, char* argv[])
{
    //因为是com所以你懂的
    HRESULT hr = CoInitialize(NULL);
    if (SUCCEEDED(hr))
    {
        IUIAutomationElement* pRoot = NULL;
        IUIAutomationElement* pFound = NULL;
        IUIAutomationCondition* pCondition = NULL;
        IUIAutomationInvokePattern* pPattern = NULL;
        do
        {
            //初始化IUAutomation实例
            hr = CoCreateInstance(CLSID_CUIAutomation, NULL, CLSCTX_INPROC_SERVER,
                IID_PPV_ARGS(&g_pIUAutomation));
            if (FAILED(hr))
            {
                printf("create IUAutomation instance failed ");
                break;
            }
            
            WCHAR *wcButtonName1 = new WCHAR[256];
            wcButtonName1 = ConvertCharToWCHAR(argv[1]);
            HWND g_hwd = (HWND)0x204a8;

            //创建条件搜索
            VARIANT varName;
            varName.vt = VT_BSTR;
            varName.bstrVal = SysAllocString(wcButtonName1);

            //这个是根据句柄获得该 句柄下所有的元素，我注释掉了
            //hr = g_pIUAutomation->ElementFromHandle(g_hwd,&pRoot);
            //获得根目录下，就是从开始 下所有控件元素
            hr = g_pIUAutomation->GetRootElement(&pRoot);
            if (FAILED(hr) || pRoot == NULL)
            {
                printf("get root element failed/n");
                break;
            }

            //创建条件搜索，根据我们button名字搜索
            hr = g_pIUAutomation->CreatePropertyCondition(UIA_NamePropertyId, varName, &pCondition);
            if (FAILED(hr))
            {
                printf("create condition  failed/n");
                break;
            }
            show_pRoot(hr, pRoot, pCondition);
            pRoot->FindFirst(TreeScope_Subtree, pCondition, &pFound);


            if (pFound != NULL) {
                //调用invoke点击
                hr = pFound->GetCurrentPatternAs(UIA_InvokePatternId, IID_PPV_ARGS(&pPattern));
                
                RECT boundingRect;
                hr = pFound->get_CurrentBoundingRectangle(&boundingRect);
                if (SUCCEEDED(hr)) printf("%d,%d,%d,%d\n", boundingRect.left, boundingRect.top, boundingRect.right, boundingRect.bottom);
                
                if (pPattern != NULL) {
                    hr = pPattern->Invoke();
                }
                else {
                    //添加模拟按钮，点击boundingRect
                    // 模拟鼠标点击控件的中心位置
                    int centerX = (boundingRect.left + boundingRect.right) / 2;
                    int centerY = (boundingRect.top + boundingRect.bottom) / 2;
                    SimulateMouseClick(centerX, centerY);
                }
            }

        } while (FALSE);

        if (g_pIUAutomation != NULL)
        {
            g_pIUAutomation->Release();
            g_pIUAutomation = NULL;
        }
        if (pRoot != NULL)
        {
            pRoot->Release();
            pRoot = NULL;
        }
        if (pFound != NULL)
        {
            pFound->Release();
            pFound = NULL;
        }
        if (pCondition != NULL)
        {
            pCondition->Release();
            pCondition = NULL;
        }
        if (pPattern != NULL)
        {
            pPattern->Release();
            pPattern = NULL;
        }
        CoUninitialize();
    }
    else
    {
        printf("initialize com failed/n");
    }
    return 0;
}