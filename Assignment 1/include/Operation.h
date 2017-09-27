#ifndef OPERATION_H
#define OPERATION_H


sealed class Operation
{
    public static readonly Operation Name1 = new Operation("Name1");
    public static readonly Operation Name2 = new Operation("Name2");

    private Operation(string value)
    {
        Value = value;
    }

    public string Value { get; private set; }
}
#endif // OPERATION_H
