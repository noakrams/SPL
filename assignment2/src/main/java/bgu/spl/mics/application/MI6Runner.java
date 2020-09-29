package bgu.spl.mics.application;

import bgu.spl.mics.MessageBrokerImpl;
import bgu.spl.mics.application.passiveObjects.Diary;
import bgu.spl.mics.application.passiveObjects.Inventory;
import bgu.spl.mics.application.passiveObjects.Squad;
import bgu.spl.mics.application.publishers.TimeService;
import bgu.spl.mics.application.subscribers.Intelligence;
import bgu.spl.mics.application.subscribers.M;
import bgu.spl.mics.application.subscribers.Moneypenny;
import bgu.spl.mics.application.subscribers.Q;
import com.google.gson.Gson;
import com.google.gson.stream.JsonReader;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.concurrent.CountDownLatch;


/**
 * This is the Main class of the application. You should parse the input file,
 * create the different instances of the objects, and run the system.
 * In the end, you should output serialized objects.
 */
public class MI6Runner {
    public static void main(String[] args) {
        Gson gson = new Gson();
        try {
            JsonReader reader = new JsonReader(new FileReader(args[0]));
            GsonObject obj = gson.fromJson(reader, GsonObject.class);
            Inventory inventory = Inventory.getInstance();
            inventory.load(obj.inventory);
            Squad squad = Squad.getInstance();
            squad.load(obj.squad);
            //count the number of threads, +1 for Q subscriber
            int nOfThreads = obj.services.M + obj.services.Moneypenny + obj.services.intelligence.length + 1;
            CountDownLatch latch = new CountDownLatch(nOfThreads);
            M[] m = new M[obj.services.M];
            //loops for insert the subscribers to their arrays
            for (int i = 0; i < m.length; i++) {
                m[i] = new M(Integer.toString(i + 1), latch);
            }
            Moneypenny[] mp = new Moneypenny[obj.services.Moneypenny];
            for (int i = 0; i < mp.length; i++) {
                mp[i] = new Moneypenny(Integer.toString(i + 1), latch);
            }
            Intelligence[] intelligences = new Intelligence[obj.services.intelligence.length]; // Final Intelligences Array
            for (int i = 0; i < intelligences.length; i++) {
                intelligences[i] = new Intelligence(obj.services.intelligence[i].getMissions(), "Intelligence: " + Integer.toString(i + 1), latch);
            }

            int count = 0;
            Q q = new Q(latch);
            Thread[] threads = new Thread[nOfThreads]; //threads array
            threads[count] = new Thread(q);
            count++;
            //loops that insert the threads to threads array
            for (int i = 0; i < mp.length; i++) {
                threads[count++] = new Thread(mp[i]);
            }
            for (int i = 0; i < m.length; i++) {
                threads[count++] = new Thread(m[i]);
            }
            for (int i = 0; i < intelligences.length; i++) {
                threads[count++] = new Thread(intelligences[i]);
            }
            for (int i = 0; i < threads.length; i++) {
                threads[i].start();
            }
            //count down to 0 until all subscribers are initialized

            latch.await();
            //timeService Thread
            TimeService timeService = new TimeService(obj.services.time);
            Thread t1 = new Thread(timeService);
            t1.start();
            //waiting for all Threads to copmplete their selves before continue with the main thread
            for (Thread thread : threads) {
                thread.join();
            }
            Diary diary = Diary.getInstance();
            inventory.printToFile(args[1]);
            diary.printToFile(args[2]);
        } catch (FileNotFoundException | InterruptedException ignored) {
        }
    }
}