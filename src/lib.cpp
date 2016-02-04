#include "lib.h"

#ifdef AVR
#include <avr/pgmspace.h>
#endif

namespace Mal_Lib
{
Led::Led(uint8_t pinNumber, bool autoInitOutput) : DigitalPin(pinNumber)
{
    // I'd expect LEDs 99% of the time want to be in OUTPUT mode
	// TODO: notate... again not sure why i need to exclude PSoC here...
	// could be cuz of global init conflicts
#if !CYPRESS_PSOC && !WIN32
    if(autoInitOutput)
      SetMode(OUTPUT);
#endif
}

Led::Led() : Led(13, true)
{

}

void Profiler::Profile()
{
#ifdef AVR
  uint32_t currentTimestamp = micros();
#else
  uint32_t currentTimestamp = 0; // FIX: Obviously not yet functional
#endif
  uint16_t interval = currentTimestamp - lastTimestamp;

  lastTimestamp = currentTimestamp;
  if(iterator == 0xFFFF)
    overflowed = true;
  iterator++;

  if(interval > max)
  {
    max = interval;
    maxs.put(interval);
    if(spikeObserver != NULL && !spikeObserverLatch)
    {
      spikeObserver(*this);
      // spike observers tend to be time consuming, creating a subsequent spike
      // set a latch so that we don't catch the next spike and observe it too, creating
      // a "batch" of spike observers
      spikeObserverLatch = true;
      // reset this latch when we hit our frequency counter
    }
  }
  if(interval < min)
    min = interval;

  if(++frequencyCounter >= frequency)
  {
    spikeObserverLatch = false;
    buffer.put(interval);
    frequencyCounter = 0;
  }
}

uint16_t Profiler::getAverage()
{
  byte i;
  uint32_t average = 0;

  for(i = 0; i < profileCount; i++)
  {
    average += intervals[i];
  }

  return average /= profileCount;
}


/*
template <class T>
T CircularBuffer<T>::get()
{
  T value = peek();
  incrementPositionGet();
  return value;
}
*/

void ObservableArray::notify(IObservable* observable)
{
  IObserver** currentObserver = (IObserver**)observers;
  for(byte i = length; i-- > 0;)
  {
    (*currentObserver)->observe(observable);
    currentObserver++;
  }
}

// this presumes there's room to grow in the array! be careful!!
void ObservableArray::add(IObserver* observer)
{
  observers[length++] = observer;
}


void ObserverArray::observe(IObservable* subject)
{
  notify(subject);
}



}
