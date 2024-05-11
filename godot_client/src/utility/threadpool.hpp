#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <godot_cpp/classes/node.hpp>

namespace godot{

	class Thread: public Node{
        private:
            bool destroying = false;
            std::thread worker;
            std::queue<std::function<void()>> jobQueue;
            std::mutex queueMutex;
            std::condition_variable condition;

            void queueLoop(){
                while(true){
                    std::function<void()> job;
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        condition.wait(lock, [this] { return !jobQueue.empty() || destroying; });
                        if (destroying)
                        {
                            break;
                        }
                        job = jobQueue.front();
                    }

                    job();

                    {
                        std::lock_guard<std::mutex> lock(queueMutex);
                        jobQueue.pop();
                        condition.notify_one();
                    }
                }
            }

        public:
            Thread(){
                worker = std::thread(&Thread::queueLoop, this);
            }

            ~Thread(){
                if(worker.joinable()){
                    wait();
                    queueMutex.lock();
                    destroying = true;
                    condition.notify_one();
                    queueMutex.unlock();
                    worker.join();
                }
            }

            bool isJobQueueEmpty(){
                return jobQueue.empty();
            }

            size_t getJobQueueSize(){
                return jobQueue.size();
            }

            void addJob(std::function<void()> function){
                std::lock_guard<std::mutex> lock(queueMutex);
                jobQueue.push(std::move(function));
                condition.notify_one();
            }

            void wait(){
                std::unique_lock<std::mutex> lock(queueMutex);
                condition.wait(lock, [this]() { return jobQueue.empty(); });
            }
    };
        
	class ThreadPool: public Node{
        public:
            std::vector<std::unique_ptr<Thread>> threads;
            int clientThread = -1;

            void setThreadCount(uint32_t count){
                threads.clear();
                for (auto i = 0; i < count; i++){
                    threads.push_back(std::make_unique<Thread>());
                }
            }

            int getEmptyThread(){
                for(auto i = 0; i < threads.size(); i++){
                    if(threads[i]->isJobQueueEmpty()) return i;
                }
                return -1;
            }

            int getLeastBusyNonClientThread(){
                size_t size = SIZE_MAX;
                int t = -1;
                for(auto i = 0; i < threads.size(); i++){
                    if(i == clientThread) continue;
                    size_t s = threads[i]->getJobQueueSize();
                    if(size >= s){
                        size = s;
                        t = i;
                    }
                    if(size == 0) break;
                }
                return t;
            }

            int getLeastBusyThread(){
                size_t size = SIZE_MAX;
                int t = -1;
               for(auto i = 0; i < threads.size(); i++){
                    size_t s = threads[i]->getJobQueueSize();
                    if(size >= s){
                        size = s;
                        t = i;
                    }
                    if(size == 0) break;
                }
                return t;
            }

            int getNonClientThread(){
                for(auto i = 0; i < threads.size(); i++){
                    if(i == clientThread) continue;
                    return i;
                }
                return -1;
            }

            void wait(){
                for(auto &thread : threads){
                    thread->wait();
                }
            }
	};
}