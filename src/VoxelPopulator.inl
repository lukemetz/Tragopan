
template <typename VolumeType>
VoxelPopulator<VolumeType>::VoxelPopulator(std::shared_ptr<VolumeType> volume) : voxel_data(volume)
{
}

template <typename VolumeType>
void VoxelPopulator<VolumeType>::update(VoxelFunction & function, const Region & region)
{
  int workSize = 32*32*32;
  std::vector<Vec3I> remaining_offsets;
  auto t_start = std::chrono::high_resolution_clock::now();

  for (int z = 0; z < voxel_data->getDepth(); z+=32) {
    for (int y = 0; y < voxel_data->getHeight(); y+=32) {
      for (int x = 0; x < voxel_data->getWidth(); x+=32) {
        remaining_offsets.push_back(Vec3I(x,y,z));
      }
    }
  }
  auto t_end = std::chrono::high_resolution_clock::now();
  auto first_time = std::chrono::duration_cast<std::chrono::milliseconds>(t_end-t_start).count();

  t_start = std::chrono::high_resolution_clock::now();

  int num_tasks = 50;
  std::vector<std::vector<Voxel>> temps;
  std::mutex done_mutex;
  enum codes {
    k_finished_task = 0,
    k_running = 1,
    k_stopped = 2
  };
  std::vector<int> done; //Store status codes
  std::vector<std::thread> threads;
  std::vector<Vec3I> on_offset;

  auto thread_func= [&temps, &function, &done_mutex, &done](int thread_index, Vec3I offset) {
    std::cout << "Offset(" << offset.getX() <<"," << offset.getY() <<","<<offset.getZ()<< ")" <<std::endl;
    
    for (auto i=temps[thread_index].begin(); i != temps[thread_index].end(); ++i) {
      int loc = i - temps[thread_index].begin();
      int x = loc%32;
      int y = (loc%(32*32)) / 32;
      int z = loc/32/32;
      function.execute(offset.getX()+x, offset.getY()+y, offset.getZ()+z, *i);
    }
    done_mutex.lock();
    done[thread_index] = k_finished_task;
    done_mutex.unlock();
  };


  std::cout << "b1" << std::endl; 
  for (int i=0; i < num_tasks; ++i) {
    done_mutex.lock();
    done.push_back(k_running);
    done_mutex.unlock();
    temps.push_back(std::vector<Voxel>( workSize, Voxel(1., Vec3F(1,1,1))));
    Vec3I offset = remaining_offsets.back();
    remaining_offsets.pop_back();
    on_offset.push_back(offset);

    auto bind_func = std::bind(thread_func, i, offset);
    threads.push_back(std::thread(bind_func));
  }

  while (!std::all_of(done.begin(), done.end(), [](int f) {return f == k_stopped;})) {

    auto found = std::find(done.begin(), done.end(), k_finished_task);
    if (found == done.end()) {
      //Still running
      continue;
    }
    int i = found - done.begin();
    threads[i].join();
    Vec3I cur_offset = on_offset[i]; 
    for (int loc=0; loc < workSize; ++loc) {
      int x = loc%32;
      int y = (loc%(32*32)) / 32;
      int z = loc/32/32;
      Voxel v = temps[i][loc];
      voxel_data->setVoxelAt(cur_offset.getX() + x, cur_offset.getY() + y, cur_offset.getZ() + z, v);
    }
    

    if (!remaining_offsets.empty()) {
      Vec3I offset = remaining_offsets.back();
      remaining_offsets.pop_back();
      on_offset[i] = offset;
      
      auto bind_func = std::bind(thread_func, i, offset);
      done_mutex.lock();
      done[i] = k_running;
      done_mutex.unlock();
      threads[i] = std::thread(bind_func);
    } else {
      std::cout << "Stopped thread" << i << std::endl;
      done_mutex.lock();
      done[i] = k_stopped;
      done_mutex.unlock();
    }
  }
  
  for (auto temp : temps) {
    std::cout << temp[0].getDensity() << temp[199].getDensity() << std::endl;
  }


  t_end = std::chrono::high_resolution_clock::now();
  auto second_time = std::chrono::duration_cast<std::chrono::milliseconds>(t_end-t_start).count();

  std::cout << "TIME DELTA is" << static_cast<float>(first_time)/second_time << std::endl;

}
