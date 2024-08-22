//#include "DelegateTest.h"
//
//#include <memory>
//#include <string>
//
//#include <Trinity/Core/Types/Delegate.h>
//
//using EventType = int;
//
//class IEvent
//{
//public:
//    IEvent(void) = default;
//    IEvent(const IEvent& p_rIEvent) = delete;
//    IEvent(const IEvent&& p_rIEvent) = delete;
//    IEvent& operator=(const IEvent& p_rIEvent) = delete;
//    IEvent& operator=(const IEvent&& p_rrIEvent) = delete;
//    virtual ~IEvent(void) = default;
//
//    virtual EventType getType(void) const = 0;
//    virtual std::string getName(void) const = 0;
//
//}; // IEvent class
//
//using EventSPtr = std::shared_ptr<IEvent>;
//
//class TestEvent
//    : public IEvent
//{
//public:
//    static const EventType Type = 0x4a6ee2cf;
//
//private:
//    int m_Data = 100;
//
//public:
//    TestEvent(void) = default;
//    virtual ~TestEvent(void) = default;
//
//    virtual EventType getType(void) const final
//    {
//        return TestEvent::Type;
//    }
//    virtual std::string getName(void) const final
//    {
//        return "TestEvent";
//    }
//
//    int getData(void) const
//    {
//        return m_Data;
//    }
//
//}; // TestEvent class
//
//
//extern void rawFreeFuncNoParamA(void);
//extern void rawFreeFuncNoParamB(void);
//extern void rawFreeFuncWithParamA(EventSPtr);
//extern void rawFreeFuncWithParamB(EventSPtr);
//
//inline void rawInlineFreeFuncNoParamA(void)
//{
//
//}
//
//inline void rawInlineFreeFuncNoParamB(void)
//{
//
//}
//
//inline void rawInlineFreeFuncWithParamA(EventSPtr evt)
//{
//    const auto name = evt->getName();
//}
//
//inline void rawInlineFreeFuncWithParamB(EventSPtr evt)
//{
//    const auto name = evt->getName();
//}
//
//class SingleInheritBaseClass
//{
//public:
//    static void rawStaticMemFuncNoParamA(void);
//    static void rawStaticMemFuncNoParamB(void);
//
//    void rawMemFuncNoParamA(void);
//    void rawMemFuncNoParamB(void);
//    void rawMemFuncWithParamA(EventSPtr);
//    void rawMemFuncWithParamB(EventSPtr);
//
//};
//
//class SingleInheritClass
//    : public SingleInheritBaseClass
//{
//public:
//    virtual void rawMemFuncNoParamA(void);
//    virtual void rawMemFuncNoParamB(void);
//    virtual void rawMemFuncWithParamA(EventSPtr);
//    virtual void rawMemFuncWithParamB(EventSPtr);
//
//};
//
/////////////////////////////////////////////////////////////////////////////////
//
//void rawFreeFuncNoParamA(void)
//{
//    int i = 1 + 1;
//    int a = i + 1;
//    int b = a * i;
//    b = b * 2;
//}
//
//void rawFreeFuncNoParamB(void)
//{
//    return;
//}
//
//void SingleInheritBaseClass::rawStaticMemFuncNoParamA(void)
//{
//
//}
//
//void SingleInheritBaseClass::rawStaticMemFuncNoParamB(void)
//{
//
//}
//
//void SingleInheritBaseClass::rawMemFuncNoParamA(void)
//{
//
//}
//
//void SingleInheritBaseClass::rawMemFuncNoParamB(void)
//{
//
//}
//
//void SingleInheritBaseClass::rawMemFuncWithParamA(EventSPtr)
//{
//
//}
//
//void SingleInheritBaseClass::rawMemFuncWithParamB(EventSPtr)
//{
//
//}
//
//void SingleInheritClass::rawMemFuncNoParamA(void)
//{
//
//}
//
//void SingleInheritClass::rawMemFuncNoParamB(void)
//{
//
//}
//
//void SingleInheritClass::rawMemFuncWithParamA(EventSPtr)
//{
//
//}
//
//void SingleInheritClass::rawMemFuncWithParamB(EventSPtr)
//{
//
//}
//
//////////////////////////////////////////////////////////////////////////////
//
//TRNT_IMPL_TEST_CASE(Types, TDelegate)
//{
//    {
//        auto inlineFreeFuncA_3 = TDelegate<void(void)>(&rawInlineFreeFuncNoParamA);
//        auto inlineFreeFuncA_4 = TDelegate<void(void)>(&rawInlineFreeFuncNoParamA);
//
//        auto singleInheritStaticmemFuncNoParamA_1 = TDelegate<void(void)>::From(&SingleInheritBaseClass::rawStaticMemFuncNoParamA);
//        auto singleInheritStaticmemFuncNoParamA_2 = TDelegate<void(void)>(&SingleInheritBaseClass::rawStaticMemFuncNoParamA);
//        TRNT_TEST_EXPECT_TRUE(inlineFreeFuncA_3 != singleInheritStaticmemFuncNoParamA_1);
//        TRNT_TEST_EXPECT_TRUE(inlineFreeFuncA_3 != singleInheritStaticmemFuncNoParamA_2);
//        TRNT_TEST_EXPECT_TRUE(singleInheritStaticmemFuncNoParamA_1 == singleInheritStaticmemFuncNoParamA_2);
//
//        auto inlineFreeFuncB_1 = TDelegate<void(void)>(&rawInlineFreeFuncNoParamB);
//        TRNT_TEST_EXPECT_TRUE(inlineFreeFuncA_3 != inlineFreeFuncB_1);
//
//        inlineFreeFuncA_3();
//        inlineFreeFuncA_4();
//
//        singleInheritStaticmemFuncNoParamA_1();
//        singleInheritStaticmemFuncNoParamA_2();
//
//        inlineFreeFuncB_1();
//    }
//
//    {
//        auto inlineFreeFuncA_1 = TDelegate<void(void)>::From<&rawInlineFreeFuncNoParamA>();
//        auto inlineFreeFuncA_2 = TDelegate<void(void)>::From<&rawInlineFreeFuncNoParamA>();
//        TRNT_TEST_EXPECT_TRUE(inlineFreeFuncA_1 == inlineFreeFuncA_2);
//        TRNT_TEST_EXPECT_TRUE(inlineFreeFuncA_2 == inlineFreeFuncA_1);
//
//        inlineFreeFuncA_1();
//        inlineFreeFuncA_2();
//
//        auto inlineFreeFuncParamA_1 = TDelegate<void(EventSPtr)>(&rawInlineFreeFuncWithParamA);
//        auto inlineFreeFuncParamA_2 = TDelegate<void(EventSPtr)>(&rawInlineFreeFuncWithParamA);
//        auto inlineFreeFuncParamB_1 = TDelegate<void(EventSPtr)>(&rawInlineFreeFuncWithParamB);
//        TRNT_TEST_EXPECT_TRUE(inlineFreeFuncParamA_1 == inlineFreeFuncParamA_2);
//        TRNT_TEST_EXPECT_TRUE(inlineFreeFuncParamA_2 == inlineFreeFuncParamA_1);
//        TRNT_TEST_EXPECT_TRUE(inlineFreeFuncParamA_1 != inlineFreeFuncParamB_1);
//        TRNT_TEST_EXPECT_TRUE(inlineFreeFuncParamB_1 != inlineFreeFuncParamA_1);
//
//        auto inlineFreeFuncA_3 = TDelegate<void(void)>(&rawInlineFreeFuncNoParamA);
//        auto inlineFreeFuncA_4 = TDelegate<void(void)>(&rawInlineFreeFuncNoParamA);
//        TDelegate<void(void)> inlineFreeFuncA_5 = &rawInlineFreeFuncNoParamA;
//        auto inlineFreeFuncA_6 = TDelegate<void(void)>::From(&rawInlineFreeFuncNoParamA);
//        TRNT_TEST_EXPECT_TRUE(inlineFreeFuncA_3 == inlineFreeFuncA_5);
//        TRNT_TEST_EXPECT_TRUE(inlineFreeFuncA_3 == inlineFreeFuncA_6);
//        TRNT_TEST_EXPECT_TRUE(inlineFreeFuncA_3 == inlineFreeFuncA_4);
//        TRNT_TEST_EXPECT_TRUE(inlineFreeFuncA_5 == inlineFreeFuncA_6);
//    }
//
//    {
//        auto freeFuncA_1 = TDelegate<void(void)>::From<&rawFreeFuncNoParamA>();
//        auto freeFuncA_2 = TDelegate<void(void)>::From<&rawFreeFuncNoParamA>();
//        TRNT_TEST_EXPECT_TRUE(freeFuncA_1 == freeFuncA_2);
//
//        auto inlineFreeFuncA_1 = TDelegate<void(void)>::From<&rawInlineFreeFuncNoParamA>();
//        auto inlineFreeFuncA_2 = TDelegate<void(void)>::From<&rawInlineFreeFuncNoParamA>();
//        TRNT_TEST_EXPECT_TRUE(inlineFreeFuncA_1 == inlineFreeFuncA_2);
//
//        auto inlineFreeFuncParamA_1 = TDelegate<void(EventSPtr)>::From<&rawInlineFreeFuncWithParamA>();
//        auto inlineFreeFuncParamA_2 = TDelegate<void(EventSPtr)>::From<&rawInlineFreeFuncWithParamA>();
//        TRNT_TEST_EXPECT_TRUE(inlineFreeFuncParamA_1 == inlineFreeFuncParamA_2);
//    }
//
//    {
//        auto freeFuncA = TDelegate<void(void)>::From<&rawFreeFuncNoParamA>();
//        auto freeFuncB = TDelegate<void(void)>::From<&rawFreeFuncNoParamB>();
//        TRNT_TEST_EXPECT_TRUE(freeFuncA != freeFuncB);
//
//        auto inlineFreeFuncA = TDelegate<void(void)>::From<&rawInlineFreeFuncNoParamA>();
//        auto inlineFreeFuncB = TDelegate<void(void)>::From<&rawInlineFreeFuncNoParamB>();
//        TRNT_TEST_EXPECT_TRUE(inlineFreeFuncA != inlineFreeFuncB);
//
//        auto inlineFreeFuncParamA = TDelegate<void(EventSPtr)>::From<&rawInlineFreeFuncWithParamA>();
//        auto inlineFreeFuncParamB = TDelegate<void(EventSPtr)>::From<&rawInlineFreeFuncWithParamB>();
//        TRNT_TEST_EXPECT_TRUE(inlineFreeFuncParamA != inlineFreeFuncParamB);
//    }
//
//    {
//        TDelegate<void(void)> lambda_1([]() { int i = 1 + 1; return; });
//        lambda_1();
//        TDelegate<void(void)> lambda_2([]() { int i = 1 + 1; return; });
//        lambda_2();
//        auto lambda_3 = TDelegate<void(void)>::From([]() { return; });
//        lambda_3();
//        TDelegate<void(void)> lambda_4 = []() { int i = 1 + 1; return; };
//        lambda_4();
//        TRNT_TEST_EXPECT_TRUE(lambda_1 != lambda_2);
//        TRNT_TEST_EXPECT_TRUE(lambda_1 != lambda_3);
//        TRNT_TEST_EXPECT_TRUE(lambda_1 != lambda_4);
//    }
//
//    {
//        auto singleInheritStaticmemFuncNoParamA_1 = TDelegate<void(void)>::From<&SingleInheritBaseClass::rawStaticMemFuncNoParamA>();
//        singleInheritStaticmemFuncNoParamA_1();
//        auto singleInheritStaticmemFuncNoParamA_2 = TDelegate<void(void)>::From<&SingleInheritBaseClass::rawStaticMemFuncNoParamA>();
//        singleInheritStaticmemFuncNoParamA_2();
//
//        TRNT_TEST_EXPECT_TRUE(singleInheritStaticmemFuncNoParamA_1 == singleInheritStaticmemFuncNoParamA_2);
//    }
//
//    {
//        auto singleInheritStaticmemFuncNoParamA = TDelegate<void(void)>::From<&SingleInheritBaseClass::rawStaticMemFuncNoParamA>();
//        auto singleInheritStaticmemFuncNoParamB = TDelegate<void(void)>::From<&SingleInheritBaseClass::rawStaticMemFuncNoParamB>();
//        TRNT_TEST_EXPECT_TRUE(singleInheritStaticmemFuncNoParamA != singleInheritStaticmemFuncNoParamB);
//    }
//
//    {
//        SingleInheritBaseClass single_inhert_base_obj;
//        auto singleInheritBMemFuncA_1 = TDelegate<void(void)>::From<SingleInheritBaseClass, &SingleInheritBaseClass::rawMemFuncNoParamA>(&single_inhert_base_obj);
//        auto singleInheritBMemFuncA_2 = TDelegate<void(void)>::From<SingleInheritBaseClass, &SingleInheritBaseClass::rawMemFuncNoParamA>(&single_inhert_base_obj);
//        TRNT_TEST_EXPECT_TRUE(singleInheritBMemFuncA_1 == singleInheritBMemFuncA_2);
//        auto singleInheritBMemFuncParamA_1 = TDelegate<void(EventSPtr)>::From<SingleInheritBaseClass, &SingleInheritBaseClass::rawMemFuncWithParamA>(&single_inhert_base_obj);
//        auto singleInheritBMemFuncParamA_2 = TDelegate<void(EventSPtr)>::From<SingleInheritBaseClass, &SingleInheritBaseClass::rawMemFuncWithParamA>(&single_inhert_base_obj);
//        TRNT_TEST_EXPECT_TRUE(singleInheritBMemFuncParamA_1 == singleInheritBMemFuncParamA_2);
//
//        SingleInheritClass single_inhert_obj;
//        auto singleInheritMemFuncA_1 = TDelegate<void(void)>::From<SingleInheritClass, &SingleInheritClass::rawMemFuncNoParamA>(&single_inhert_obj);
//        auto singleInheritMemFuncA_2 = TDelegate<void(void)>::From<SingleInheritClass, &SingleInheritClass::rawMemFuncNoParamA>(&single_inhert_obj);
//        TRNT_TEST_EXPECT_TRUE(singleInheritMemFuncA_1 == singleInheritMemFuncA_2);
//        auto singleInheritMemFuncParamA_1 = TDelegate<void(EventSPtr)>::From<SingleInheritClass, &SingleInheritClass::rawMemFuncWithParamA>(&single_inhert_obj);
//        auto singleInheritMemFuncParamA_2 = TDelegate<void(EventSPtr)>::From<SingleInheritClass, &SingleInheritClass::rawMemFuncWithParamA>(&single_inhert_obj);
//        TRNT_TEST_EXPECT_TRUE(singleInheritMemFuncParamA_1 == singleInheritMemFuncParamA_2);
//    }
//
//    {
//        SingleInheritBaseClass single_inhert_base_obj;
//        auto singleInheritBMemFuncA = TDelegate<void(void)>::From<SingleInheritBaseClass, &SingleInheritBaseClass::rawMemFuncNoParamA>(&single_inhert_base_obj);
//        auto singleInheritBMemFuncB = TDelegate<void(void)>::From<SingleInheritBaseClass, &SingleInheritBaseClass::rawMemFuncNoParamB>(&single_inhert_base_obj);
//        TRNT_TEST_EXPECT_TRUE(singleInheritBMemFuncA != singleInheritBMemFuncB);
//        auto singleInheritBMemFuncParamA = TDelegate<void(EventSPtr)>::From<SingleInheritBaseClass, &SingleInheritBaseClass::rawMemFuncWithParamA>(&single_inhert_base_obj);
//        auto singleInheritBMemFuncParamB = TDelegate<void(EventSPtr)>::From<SingleInheritBaseClass, &SingleInheritBaseClass::rawMemFuncWithParamB>(&single_inhert_base_obj);
//        TRNT_TEST_EXPECT_TRUE(singleInheritBMemFuncParamA != singleInheritBMemFuncParamB);
//
//        SingleInheritClass single_inhert_obj;
//        auto singleInheritMemFuncA = TDelegate<void(void)>::From<SingleInheritClass, &SingleInheritClass::rawMemFuncNoParamA>(&single_inhert_obj);
//        auto singleInheritMemFuncB = TDelegate<void(void)>::From<SingleInheritClass, &SingleInheritClass::rawMemFuncNoParamB>(&single_inhert_obj);
//        TRNT_TEST_EXPECT_TRUE(singleInheritMemFuncA != singleInheritMemFuncB);
//        auto singleInheritMemFuncParamA = TDelegate<void(EventSPtr)>::From<SingleInheritClass, &SingleInheritClass::rawMemFuncWithParamA>(&single_inhert_obj);
//        auto singleInheritMemFuncParamB = TDelegate<void(EventSPtr)>::From<SingleInheritClass, &SingleInheritClass::rawMemFuncWithParamB>(&single_inhert_obj);
//        TRNT_TEST_EXPECT_TRUE(singleInheritMemFuncParamA != singleInheritMemFuncParamB);
//    }
//}