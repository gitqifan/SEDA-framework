#include "stage.h"

stage::stage() {
   stage("");
}

stage::stage(string stage_name) {
   this->stage_name = stage_name;
}

string stage::get_stage_name() {
   return stage_name;
}

void stage::setResources(vector<string> &res) {
   resources = res;
   //rc->setResources(res);
}

bool stage::init(Config &config) {
   /*
    * 每个stage中的四个模块。
    */
   sq = new stage_queue(); 
   wp = new thread_worker_pool();
   rc = new receiver();
   rc->setResources(resources);
   sh = new stage_handler(rc, sq, wp);
   sc = new stage_control(sq, wp);

   
   wp->start();
   Function func_rc(rc->run, rc);
   Function func_sh(sh->run, sh);
   Function fun_sc(sc->run, sc);
   
   //rc->run();
   wp->run(func_rc);
   wp->run(func_sh);
   wp->run(fun_sc);

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
   return true;
}
