package bgu.spl.mics.application.subscribers;

import bgu.spl.mics.*;
import bgu.spl.mics.application.messages.*;
import bgu.spl.mics.application.passiveObjects.Diary;
import bgu.spl.mics.application.passiveObjects.Report;

import java.util.concurrent.CountDownLatch;


/**
 * M handles ReadyEvent - fills a report and sends agents to mission.
 * <p>
 * You can add private fields and public methods to this class.
 * You MAY change constructor signatures and even add new public constructors.
 */
public class M extends Subscriber {

    private int tick;
    private CountDownLatch latch;

    public M(String serialNumber, CountDownLatch latch) {
        super("M"+serialNumber);
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
        //call function
        subscribeEvent(MissionReceivedEvent.class, MissionReceivedEvent -> {
            Future<Integer> future = new Future<>();
            Diary.getInstance().increment(); //increment the total by 1
            AgentsAvailableEvent availableAgents = new AgentsAvailableEvent(MissionReceivedEvent.getAgentsNumber(), MissionReceivedEvent.getDuration(), future);
            Future<Boolean> agentsAvailable = getSimplePublisher().sendEvent(availableAgents);
            if (agentsAvailable.get() != null && agentsAvailable.get()) {
                GadgetAvailableEvent gadget = new GadgetAvailableEvent(MissionReceivedEvent.getGadget());
                Future<Boolean> gadgetAvailable = getSimplePublisher().sendEvent(gadget);
                if (gadgetAvailable != null && gadgetAvailable.get() != null) {
                    if (gadgetAvailable.get()) {
                        if (tick <= MissionReceivedEvent.getTimeExpired()) {
                            future.resolve(MissionReceivedEvent.getDuration());
                            complete(MissionReceivedEvent, null);
                            Report report = new Report(MissionReceivedEvent.getMissionInfo(), getName(), availableAgents.getMoneyPennySerial(), gadget.getTimeTick(), tick + MissionReceivedEvent.getDuration());
                            Diary.getInstance().addReport(report);
                            return;
                        }
                    }
                }
            }
            missionNotCompleted(MissionReceivedEvent, future);
        });
        latch.countDown();
    }

    private void missionNotCompleted(MissionReceivedEvent missionReceivedEvent, Future<Integer> future) {
        future.resolve(-1);
        complete(missionReceivedEvent, null);
    }
}


