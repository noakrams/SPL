package bgu.spl.mics;

import bgu.spl.mics.Future;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.concurrent.TimeUnit;
import static org.junit.jupiter.api.Assertions.*;

public class FutureTest {
    Future<Integer> test;
    Future<String> test1;
    Future<String> test2;

    @BeforeEach
    public void setUp(){
        test = new Future<>();
        test1 = new Future<>();
    }

    @Test
    public void isDone() {
        assertFalse(test.isDone());
        test.resolve(999);
        assertTrue(test.isDone());
    }


    @Test
    public void get() {
        assertNull(test.get());
        assertNull(test1.get());

        test.resolve(6);
        assertEquals(new Integer(6), test.get());

        test1.resolve("Game Of Thrones");
        assertEquals("Game Of Thrones", test1.get());
    }

    @Test
    public void resolve() {
        test.resolve(893);
        assertEquals(new Integer(893), test.get());
    }



    @Test
    public void get1() {
        TimeUnit time = TimeUnit.MILLISECONDS;
        Object o = null;
        o = test.get(666,time);
        assertNull(o);

        test.resolve(new Integer(5));
        assertEquals(new Integer(5), test1.get(893, time));
    }

}