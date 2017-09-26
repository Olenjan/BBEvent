/*


//typedef std::map<void*, std::vector<CReceiverBase*>>							t_ReceiverParentRegistry;			//	<CBaseReceiver*

//t_ReceiverParentRegistry			m_ParentRegistry;

//! returns true when update available
//!		for that type
//template<typename Te, typename T>
//bool getSyncedUpdate(T* t);

//non threaded handle calls out functions and sets updated false
//threaded handle locks all receivers and sets updated false
//! Locks all receivers
//template<typename Tr>
//void sync(Tr* t);


//! Unlocks all receivers, once already locked
//! TODO: only for threaded eventManager
//template<typename Tr>
//void unlock(Tr* t);


/*
//Templates should be defined in header file.
template<typename Tr>
void CEventManager::sync(Tr* t)
{
const std::type_info* p = &typeid((void*)t);
for (auto r : m_ParentRegistry[(void*)t]) //m_ParentRegistry[&typeid((void*)t)])
{
r->lock();
}
m_Synced = true;
}
template<typename Tr>
void CEventManager::unlock(Tr* t)
{
const std::type_info* p = &typeid(t);
for (auto r : m_ParentRegistry[(void*)t])//m_ParentRegistry[&typeid(t)])
{
r->unlock();
}
m_Synced = false;
}
template<typename Te, typename T>
bool CEventManager::getSyncedUpdate(T* t)
{
if (m_Synced)
{
sync((void*)t);
}
CReceiverBase* baseReceiver = (CReceiverBase*)(CReceiverInterface<Te>*)t;
//const std::type_info* p = 0; //&typeid(t);
//for (CReceiverBase* currentRec : m_ParentRegistry[(void*)t]);//[&typeid(t)])
for (auto r : m_ParentRegistry[(void*)t])
{
if (r == baseReceiver)
{
if (r->isUpdate())
{
return true;
}
}
}
return false;
}




//m_ParentRegistry[(void*)(receiver)].push_back(baseReceiver);
*/