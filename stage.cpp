#include "stage.h"

stage::stage() {
   stage("");
}

stage::stage(string stage_name) {
   this->stage_name = stage_name;
   this->reg_mem = (void **)malloc(sizeof(void *) * REG_MEM);
   memset(this->reg_mem, 0, sizeof(void *) * REG_MEM);
}

stage::~stage() {
   for (int i = 0; i < REG_MEM; ++i) {
      if (reg_mem[i]) {
         free(reg_mem[i]);
      }
   }
   free(reg_mem);
}

string stage::get_stage_name() {
   return stage_name;
}

/*
void stage::setPersistData(string key, void *data) {
   persistData.insert(pair<string, void *>(key, data));
}

void *stage::getPersistData(string key){
   return persistData[key];
}
*/

bool stage::init(Config &config) {
   /*
    * 每个stage中的四个模块。
    */
   sq = new stage_queue(); 
   wp = new thread_worker_pool();
   rc = new receiver();
   rc->setResources(resources);
   sh = new stage_handler(rc, sq, wp);
   sh->setAddr(des);
   sc = new stage_control(sq, wp);

   
   wp->start();
   Function func_rc(receiver::run, rc);
   Function func_sh(stage_handler::run, sh);
   Function fun_sc(stage_control::run, sc);
   //LogUtil::debug("running : %d", sc->getRunning());
   
   //rc->run();
   wp->run(func_rc);
   wp->run(fun_sc);
   wp->run(func_sh);

   //ec = new event_core(config);
   return true;
}

bool stage::run() { 
   /*
    * 这里从event_core中启动监听。并将EPOLL_IN事件返回的事件结构中获取IElement元素, 传递给stage_queue。
    */
   /*
   int ret = ec->init();
   if (!ret) {
      LogUtil::debug("stage::run [ec->init] error");
      return false;
   }
   ec->run();
   */
   while (true) {}
   return true;
}

   /*
void stage::loop() {
   rc->wait();
   sh->wait();
   sc->wait();
}
   */
