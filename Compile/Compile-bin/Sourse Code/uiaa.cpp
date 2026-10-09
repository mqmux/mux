#include <Windows.h>
#include <UIAutomation.h>
#include <iostream>

#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "UIAutomationCore.lib")



IUIAutomation* g_pIUAutomation = NULL;
long long glonum = 0;

void TraverseElements(IUIAutomationElement* pElement)
{

    if (!pElement) return;

    IUIAutomationCondition* pCondition = NULL;
    IUIAutomationElementArray* pChildren = NULL;
    HRESULT hr = S_OK;

    // 创建一个条件，获取所有子元素
    hr = g_pIUAutomation->CreateTrueCondition(&pCondition);
    if (FAILED(hr))
    {
        std::cout << "Failed to create condition." << std::endl;
        return;
    }

    // 查找当前元素的所有子元素
    hr = pElement->FindAll(TreeScope_Children, pCondition, &pChildren);
    if (FAILED(hr) || !pChildren)
    {
        std::cout << "Failed to find child elements." << std::endl;
        pCondition->Release();
        return;
    }

    // 获取子元素的数量
    int count = 0;
    hr = pChildren->get_Length(&count);
    if (FAILED(hr))
    {
        std::cout << "Failed to get children count." << std::endl;
        pCondition->Release();
        pChildren->Release();
        return;
    }

    // 遍历所有子元素
    for (int i = 0; i < count; ++i)
    {
        IUIAutomationElement* pChild = NULL;
        hr = pChildren->GetElement(i, &pChild);
        if (pChild)
        {
            // 获取控件名称和类型
            BSTR bstrName = NULL;
            hr = pChild->get_CurrentName(&bstrName);
            if (SUCCEEDED(hr) && bstrName)
            {
                printf("Element %d Name: %ls\n", i, bstrName);
                SysFreeString(bstrName);
            }

            int controlType = 0;
            hr = pChild->get_CurrentControlType(&controlType);
            if (SUCCEEDED(hr))
            {
                glonum++;
                printf("%lld : ", glonum);
                std::cout << "Control Type: " << controlType << std::endl;
            }
            pChild->get_CurrentControlType(&controlType);
            printf("Control Type: %d ", controlType);

            RECT boundingRect;
            hr = pChild->get_CurrentBoundingRectangle(&boundingRect);
            if (SUCCEEDED(hr)) printf("%d,%d,%d,%d\n", boundingRect.left, boundingRect.top, boundingRect.right, boundingRect.bottom);

            // 递归遍历子元素
            TraverseElements(pChild);

            pChild->Release();
        }
    }

    pCondition->Release();
    pChildren->Release();
}

int main()
{
    HRESULT hr = CoInitialize(NULL);
    if (SUCCEEDED(hr))
    {
        hr = CoCreateInstance(CLSID_CUIAutomation, NULL, CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&g_pIUAutomation));
        if (SUCCEEDED(hr))
        {
            IUIAutomationElement* pRoot = NULL;
            hr = g_pIUAutomation->GetRootElement(&pRoot);
            if (SUCCEEDED(hr) && pRoot)
            {
                // 遍历根元素下的所有控件元素
                TraverseElements(pRoot);

                pRoot->Release();
            }
            else
            {
                std::cout << "Failed to get root element." << std::endl;
            }

            g_pIUAutomation->Release();
        }
        else
        {
            std::cout << "Failed to create UI Automation instance." << std::endl;
        }

        CoUninitialize();
    }
    else
    {
        std::cout << "Failed to initialize COM." << std::endl;
    }

    return 0;
}
