package bgu.spl.mics.application.publishers;

import bgu.spl.mics.Publisher;
import bgu.spl.mics.application.messages.TickBroadcast;
import java.util.Timer;
import java.util.TimerTask;


/**
 * TimeService is the global system timer There is only one instance of this Publisher.
 * It keeps track of the amount of ticks passed since initialization and notifies
 * all other subscribers about the current time tick using {@link TickBroadcast}.
 * This class may not hold references for objects which it is not responsible for.
 * <p>
 * You can add private fields and public methods to this class.
 * You MAY change constructor signatures and even add new public constructors.
 */
public class TimeService extends Publisher {

    private Timer timer;
    private TimerTask timerTask;
    private int tick;
    private final int speed;

    public TimeService(int duration) {
        super("TimeService");
        tick = 0;
        speed = 100;
        timer = new Timer();
        timerTask = new TimerTask() {
            @Override
            public void run() {
                if (tick == duration) {
                    terminate();
                    this.cancel(); //cancel the timerTask
                    timer.cancel();
                }
                //increments by one
                else {
                    getSimplePublisher().sendBroadcast(new TickBroadcast(++tick, duration));
                }
            }
        };
    }
    @Override
    protected void initialize() {
    }

    @Override
    public void run() {
        timer.schedule(timerTask, 0, speed); //starts the timer task
        terminate();
    }
}
