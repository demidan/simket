module test();
    typedef chandle qubit;
    import "DPI-C" context function qubit new_qubit();
    import "DPI-C" context function int reset_qubit(qubit q);
    
    import "DPI-C" context function int   X   (qubit q);
    import "DPI-C" context function int   Y   (qubit q);
    import "DPI-C" context function int   Z   (qubit q);
    import "DPI-C" context function int   S   (qubit q);
    import "DPI-C" context function int   T   (qubit q);
    import "DPI-C" context function int   H   (qubit q);
    import "DPI-C" context function int   Swap(qubit q);
    
    import "DPI-C" context function int  cX   (qubit c, qubit t);
    import "DPI-C" context function int  cY   (qubit c, qubit t);
    import "DPI-C" context function int  cZ   (qubit c, qubit t);
    import "DPI-C" context function int  cS   (qubit c, qubit t);
    import "DPI-C" context function int  cT   (qubit c, qubit t);
    import "DPI-C" context function int  cH   (qubit c, qubit t);
    import "DPI-C" context function int  cSwap(qubit c, qubit t);
    
    import "DPI-C" context function int ccX   (qubit c1, qubit c0, qubit t);
    import "DPI-C" context function int ccY   (qubit c1, qubit c0, qubit t);
    import "DPI-C" context function int ccZ   (qubit c1, qubit c0, qubit t);
    import "DPI-C" context function int ccS   (qubit c1, qubit c0, qubit t);
    import "DPI-C" context function int ccT   (qubit c1, qubit c0, qubit t);
    import "DPI-C" context function int ccH   (qubit c1, qubit c0, qubit t);
    import "DPI-C" context function int ccSwap(qubit c1, qubit c0, qubit t);
    
    
endmodule : test
