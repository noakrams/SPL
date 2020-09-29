package bgu.spl.mics.application.passiveObjects;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

import java.io.*;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;


/**
 * Passive object representing the diary where all reports are stored.
 * <p>
 * This class must be implemented safely as a thread-safe singleton.
 * You must not alter any of the given public methods of this class.
 * <p>
 * You can add ONLY private fields and methods to this class as you see fit.
 */
public class Diary implements Serializable {

    private AtomicInteger total;
    private List<Report> reportList;

    //Singleton
    private static class DiaryHolder {
        private static Diary instance = new Diary();
    }

    //constructor
    private Diary() {
        this.reportList = new LinkedList<Report>();
        this.total = new AtomicInteger(0);
    }

    /**
     * Retrieves the single instance of this class.
     */
    public static Diary getInstance() {
        return DiaryHolder.instance;
    }

    private List<Report> getReports() {
        return reportList;
    }

    /**
     * adds a report to the diary
     *
     * @param reportToAdd - the report to add
     */
    public void addReport(Report reportToAdd) {
        synchronized (reportList) {
            reportList.add(reportToAdd);
        }
    }

    /**
     * <p>
     * Prints to a file name @filename a serialized object List<Report> which is a
     * List of all the reports in the diary.
     * This method is called by the main method in order to generate the output.
     */
    public void printToFile(String filename) {
        try (Writer writer = new FileWriter(filename)) {
            Gson gson = new GsonBuilder().setPrettyPrinting().create();
            gson.toJson(this, writer);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }


    /**
     * Gets the total number of received missions (executed / aborted) be all the M-instances.
     *
     * @return the total number of received missions (executed / aborted) be all the M-instances.
     */
    public int getTotal() {
        return total.get();
    }

    
    public void increment() {
        total.incrementAndGet();
    }
}
