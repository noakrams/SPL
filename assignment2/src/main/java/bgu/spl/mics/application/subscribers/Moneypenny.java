package bgu.spl.mics.application.subscribers;

import bgu.spl.mics.Subscriber;
import bgu.spl.mics.application.messages.AgentsAvailableEvent;
import bgu.spl.mics.application.messages.TickBroadcast;
import bgu.spl.mics.application.passiveObjects.Squad;

import java.util.ArrayList;
import java.util.concurrent.CountDownLatch;

/**
 * Only this type of Subscriber can access the squad.
 * Three are several Moneypenny-instances - each of them holds a unique serial number that will later be printed on the report.
 * <p>
 * You can add private fields and public methods to this class.
 * You MAY change constructor signatures and even add new public constructors.
 */
public class Moneypenny extends Subscriber {

    private CountDownLatch latch;
    private int tick;

    public Moneypenny(String serialNum, CountDownLatch latch) {
        super("Moneypenny" + serialNum);
        this.latch = latch;
    }

    @Override
    protected void initialize() {
        subscribeBroadcast(TickBroadcast.class, tickBroadcast -> {
            tick = tickBroadcast.getCurrTick();
            if (tickBroadcast.getDuration() == tick) {
                terminate();
            }
        });
        subscribeEvent(AgentsAvailableEvent.class, AgentsAvailableEvent -> {
            ArrayList<String> agentsSerials = AgentsAvailableEvent.getAgentsSerials();
            boolean getAgents = Squad.getInstance().getAgents(agentsSerials);
            complete(AgentsAvailableEvent, getAgents);
            if (getAgents) {
                int releaseOrSend = AgentsAvailableEvent.getFuture().get();
                Squad.getInstance().sendAgents(agentsSerials, releaseOrSend);
            }
        });

        latch.countDown();
    }

}
