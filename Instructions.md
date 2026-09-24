Do not code, I am about to write a software with the following features, 
 1 - main language is C++17 or 20, and the whole software is based on Qt, use snake-case for member varibles, camel case for member functions and pascal-case for class names,
 2 - we use pybind11, to expose those features/functions in the C++ code to python scripting, 
 3 - The design is event based, we have a class called Event, i.e. there is a flow of incoming , outcoming events that must be handled properly, 
 4 - For the flow of events we have 16 channels indexed 0-15, which are bi-directionl, i.e. could be used for sending and receiving events. 
 4 - The event containers  are threaded ring-buffer ( disruptor-buffers), 
 5 - The buffers are either spsc or spmc or mpmc. 
 6 - The transmits/sends, for now are done through a USB port here after we call it usb-gate.
 7 - For the usb-gate we have a class UsbGate which contains a mpsp ring-buffer, the mpsp-buffer will be used by channels (see below) to push the events to the usb-gate.
 8 - The data field within each event contains fixed 64 bits (8*8 bytes) of data here we call it DataPacket, DataPacketType = std::arrray<uint8_t,8>.
 9 - We have a class of which hanldles the events corresponding to that channel, class name = Channel.
 10- We have a singleton class which contains general data, class name = InstanceData.
 11 - Each DataPacket has its first 4 bits being decoded as an index 0-15 hereafter we call it channel_index, therefore we have 16 channels, std::array<Channel,16> channels within  InstanceData.
 12- Each Channel has its own threaded ring-buffers to handle incomming events. 
 13- The UsbGate dispaches each pocket of data to the corresponding Channel through channel_index.
 14- The UsbGate directly pushes the data to Channel's ring buffer. 
 15- Each Channle sends the data by pushing the Event's data to UsbGate ring-buffer. 
 16- The Channel class has some functionallities of which are ran through functoins of which run a member std::funcnional/lambda here we mention it as procedure, The actual name of member function which runs those
     procedure could run, there are . 
 17- At each instance of time only one procedure is ran for a Channel.
 18- There are several types of work flows: 1- A a procedure starts to send an Event through specific Channel, and do not weight for any response, 2- There are procedures which send an Event and to wait 
     for specific response, through listening to that channels incomming ring-buffer, and only returns if specific response is being received, or a time out is triggered. 
     3- A non-blocking version of case 2, of which the outcome will be checked later, but the timeout is still there even the way the method is ran is not blocking.
 19- Use the following classes in side the ArincQtUI-hostonly\src\Utils\SpscRingBuffer.h 
		ArincQtUI-hostonly\src\Utils\MpmcRingBuffer.h ArincQtUI-hostonly\src\Utils\Singletone.h  ArincQtUI-hostonly\src\Hardware\DEI1016.h ArincQtUI-hostonly\src\Hardware\DEI1016.cpp 
 20- particularly ArincQtUI-hostonly\src\Hardware\DEI1016.h contains a clean code for the UsbGate and reading a DataPack.


---

## Addendum — 2026-09-17, clarifications and corrections

The specification above is left standing as written, including its errors, because the gap
between what it says and what the code does is itself the record of what had to be decided.
Corrections and clarifications are appended here rather than edited in. The reasoning behind
each is in `ArincQtUI-hostonly/docs/markdowns/DECISIONS.md`.

**A1 — item 1, language standard: C++20.** "C++17 or 20" allowed either; C++20 is chosen.
`std::jthread` gives the clean-shutdown property item 10's singleton needs, and `std::bit_cast`
makes the frame/payload conversion in A3 defined behaviour rather than a cast through `char*`.
See D1.

**A2 — item 7, "mpsp": read as MPSC, implemented as MPMC.** The topology is many-producers /
one-consumer (16 channels push, one USB thread drains). A dedicated MPSC buffer is not written:
the existing `MpmcRingBuffer` is a valid MPSC buffer when used with one consumer, and writing a
new lock-free structure to save a CAS on a path gated by USB transfer time is the wrong trade.
See D2.

**A3 — item 8, the frame byte was missing from the specification.** The data field is 64 bits
(8 bytes), written in item 8 as "(8*8 bytes)" — that arithmetic is wrong; `8*8` would be 64
*bytes*. The unit that is buffered and transmitted is the **10-byte frame**, `record_t =
std::array<uint8_t,10>` per `src/Utils/Types.h` (`FRAME_POCKET_SIZE 10`), and
`DataPacketType = std::array<uint8_t,8>` is the **payload read out of it**. It is never
transmitted alone: the two frame markers are what let the receiver resynchronise after a dropped
byte on a serial line. Without them one lost byte desynchronises the stream permanently and
every subsequent packet is misparsed into the wrong channel, silently. See D4.

**A4 — item 5, SPMC is not used.** Item 5 lists "spsc or spmc or mpmc". The design uses SPSC
and MPMC only, and no SPMC anywhere: each channel has exactly one consumer because item 17
forbids more than one procedure running at a time. See D3.

**A5 — item 16, the unfinished sentence: `runProcedure()`.** Item 16 ends mid-sentence ("The
actual name of member function which runs those procedure could run, there are ."). The name is
`runProcedure()`. It also enforces item 17's one-at-a-time invariant, and it is the sole
consumer of its channel's SPSC buffer — which is what makes A4 sound. See D6.

**A6 — item 20 is not accurate, and this changes the plan.** `DEI1016.h`/`.cpp` is a reference
for *intent and wire framing*, not a working implementation. Reading it showed:
`AUX::getChanellIndex()` — the function the 16-channel dispatch of item 13 depends on — has an
**empty body** and is called on every received frame; `DEI1016.cpp` does not compile (undeclared
`recData`, `push_back` on a type whose method is `push`); and the receive thread busy-spins at
100% CPU on a non-blocking file descriptor. Twelve defects are recorded in
`ArincQtUI-hostonly/docs/markdowns/CHANGELOG.md` §6. `UsbGate` is therefore written against the
frame layout and proven by a loopback test, rather than refactored and assumed correct because
it already existed.

**A7 — item 11's 4-bit channel index does not fit the existing wire format. OPEN.** Item 11
says the first 4 bits of a DataPacket decode as an index 0–15. The legacy instruction byte is
already fully packed: bits 0–3 instruction, bit 4 transmit/receive, bit 5 DEI, bit 6 channel —
**one bit, addressing 2 channels** — bit 7 unused. Sixteen channels need four bits, and only one
is free. Something must give: shrink the instruction field, move DEI or transmit/receive, or
split the index across bytes. This is a hardware-protocol decision, not an implementation
detail, and it blocks the framing work. See `CHANGELOG.md` §6.2.

**A8 — item 18 case 3, the non-blocking timeout.** "the timeout is still there even the way the
method is ran is not blocking" is the harder half of the requirement. The deadline is
established when the request is made and travels with it; it is not an argument to a blocking
wait, and it is not reset by polling. A request nobody polls still expires, and a caller that
polls late gets "timed out", not "still waiting".

**A9 — not specified anywhere: what happens to a late response.** A response arriving for a
channel whose procedure has already timed out and returned is a real case. Discard it, queue it
for the next procedure, or surface it as an error — all three are defensible and they are three
different behaviours. Needs an owner decision. See `WORKFLOW.md` §6.

**A10 — not specified: whether each channel gets a dedicated thread.** Item 17 requires only
that a channel runs one procedure at a time; a shared worker pool satisfies that with a
per-channel claim, and 16 mostly-idle threads is a real cost on the Raspberry Pi target the
legacy code implies. Flagged so it is not silently decided by whoever writes `Channel` first.
See `WORKFLOW.md` §5.

   