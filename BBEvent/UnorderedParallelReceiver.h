#ifndef BB_UNORDERED_PARALLEL_RECEIVER_H
#define BB_UNORDERED_PARALLEL_RECEIVER_H

namespace BB
{

	//! Disable EventSequencer for CUnorderedParallel Receiver to have any effect

	//! processing result data is stored in receiver m_Done map due to CEvent being constant
	//! Otherwise unordered queue gives the same effect
	template<class E>
	class CReceiver<E>::CUnorderedParallel : public CInterface
	{
	private:
		typedef void t_DataType;
		std::mutex m_DoneMapMutex;
		std::map<const std::shared_ptr<E>, std::shared_ptr<t_DataType>> m_Done;
		std::vector<std::pair<std::shared_ptr<E>, std::shared_ptr<t_DataType>>> ordered;
	public:


		//Collects after simulated heavy work in offthread (in main thread)
		//FIFO - first in first out, not sequenced
		//std::map<const std::shared_ptr<CMouseDragMoveEvent>, std::shared_ptr<void>> doneData = CReceiver<CMouseDragMoveEvent>::CUnorderedParallel::consumeProcessed();
		/* sorter example - does not wait to provide serialized data
		typedef std::pair<std::shared_ptr<CMouseDragMoveEvent>, std::shared_ptr<void>> t_Type;
		auto sorterFunc = [=](t_Type l, t_Type r) -> bool
		{return l.first->getOrderNumber() < r.first->getOrderNumber(); };
		
		template<typename T>
		std::vector<std::pair<std::shared_ptr<E>, std::shared_ptr<t_DataType>>> consumeOrdered(std::function<bool(T l, T r)> sortFunc)
		{
			std::vector<std::pair<std::shared_ptr<E>, std::shared_ptr<t_DataType>>> result;
			
			std::map<const std::shared_ptr<E>, std::shared_ptr<t_DataType>> done;

			m_DoneMapMutex.lock();
			done = m_Done;
			m_Done.clear();
			m_DoneMapMutex.unlock();

			for (auto d : done)
			{
				result.push_back(d);
			}

			std::sort(result.begin(), result.end(), sortFunc);

			return result;
		}*/

		std::map<const std::shared_ptr<E>, std::shared_ptr<t_DataType>> consumeProcessed()
		{
			std::map<const std::shared_ptr<E>, std::shared_ptr<t_DataType>> done;

			m_DoneMapMutex.lock();
			done = m_Done;
			m_Done.clear();
			m_DoneMapMutex.unlock();

			return done;
		}

		
		virtual std::shared_ptr<t_DataType> process(const std::shared_ptr<E> event) = 0;

		virtual bool Event(const std::shared_ptr<E> event) final;
	};

	template<class E>
	bool CReceiver<E>::CUnorderedParallel::Event(const std::shared_ptr<E> event)
	{
		std::shared_ptr<t_DataType> done = process(event);

		//after process done, add to done map
		m_DoneMapMutex.lock();
		m_Done[event] = done;
		m_DoneMapMutex.unlock();
		return true
	}
}

#endif