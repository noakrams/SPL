package bgu.spl.mics.application.messages;

import bgu.spl.mics.Event;
import bgu.spl.mics.Future;

import java.util.ArrayList;
import java.util.List;

public class AgentsAvailableEvent implements Event<Boolean> {

    private ArrayList<String> agentsSerials;
    private int duration;
    private boolean beenSent;
    private String moneyPenny;
    private Future<Integer> future;


    public AgentsAvailableEvent(ArrayList<String> agentsSerials, int duration, Future<Integer>future){
        this.agentsSerials = agentsSerials;
        this.duration = duration;
        beenSent = false;
        this.future = future;

    }
    public Future<Integer> getFuture(){
        return future;
    }

    public void setMoneyPenny(String numMoneyPenny)
    {
        moneyPenny = numMoneyPenny;
    }

    public String getMoneyPennySerial()
    {
        return moneyPenny;
    }

    public ArrayList<String> getAgentsSerials() {
        return agentsSerials;
    }

    public int getDuration(){
        return duration;
    }

    public boolean isBeenSent() {
        return beenSent;
    }

    public void setSent() {
        beenSent = true;
    }

    public void setNotSent(){
            beenSent = false;
    }
}
