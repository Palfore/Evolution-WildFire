#ifndef TESTSUITE_H
#define TESTSUITE_H

class TestSuite {
    public:
        TestSuite();
        virtual ~TestSuite();
    private:
        void connectionTest();
};

#endif // TESTSUITE_H
