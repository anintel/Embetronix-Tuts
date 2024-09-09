# Wait Queue
A data structure used to manage the waiting of threads and processes for execution.

- Whenever a process waits it goes for sleep
    - Thus releasing the resources for other processes

## 3 steps of waitqueue
1. Initializing waitqueue
2. Queueing, putting the task to sleep
3. Waking up the task.

`The wait.h header file includes the methods for waitqueue mimplmentation`

## Initialization

### Two methods of initialization
1. Static Method
    - `DECLARE_WAIT_QUEUE_HEAD(wq)` - where wq is the name of the queue in which the task will be put to sleep
2. Dynamic Method
    - `init_waitqueue_head(&wq)` 

## Queueing 

1. `wait_event(wq, condition)` - sleep until a condition gets true
    - `wq` to wait on
    - `condition` to wait for

2. `wait_event_timeout(wq, condition, timeout)` - sleep until condition becomes true or till time runs out

3. `wait_event_cmd(wq, condition, cmd1, cmd2)` 
    - here `cmd1` command to be executed before sleep
    - `cmd2` command to be executed after sleep

* all the first 3 puts the  task to sleep in TASK_UNINTERUPTIBLE

4. `wait_event_interruptible(wq, condition)`
    - task is put to sleep (TASK_INTERUPTIBLE)

5. `wait_event_interruptible_timeout(wq, condition, timeout)`
    - timeout in jiffies 

* Jiffy - clock maintained by kernel, its value is determined by the value of the kernel constant HZ

6. `wait_event_killable(wq, condition)`
    - task is put to sleep in TASK_KILLABLE

## Waking up a queued task

1. wake_up 
    - wakes up only one process which is in non-interruptible sleep.

2. wake_up_all
    - wakes up all the processess on the queue

3. wake_up_interruptible 
    - wake up only one interruptible process from sleep.

* wake_up call causes immediate reschedule to happen, i.e. other process might run before wake_up returns.

4. wake_up_sync & wake_up_immediate_sync 
    - makes any awakened process runable but does not reschedule the CPU.

