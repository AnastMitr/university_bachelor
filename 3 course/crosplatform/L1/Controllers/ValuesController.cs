using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using Lab1Markina.Models;
using Microsoft.AspNetCore.Mvc;

namespace Lab1Markina.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class ValuesController : ControllerBase
    {   public static List<paporotnik> greenhouse_paporotnik = new List<paporotnik>();
        public static List<pixta> greenhouse_pixta = new List<pixta> ();
        static Random rnd = new Random();
        
        // GET api/values
        [HttpGet]
        public string Get()
        {           
            return "Hello, user!";
        }

        // GETapi/values/Addfromfile
        [HttpGet("Addfromfile")]
        public string AddFromFile()
        {
            int count_plants;
            using (StreamReader fin = new StreamReader("paporotnik.txt"))
            {
                count_plants = Convert.ToInt32(fin.ReadLine());

                for(int i = 0; i < count_plants; i++)
                {
                    var p = new paporotnik();
                    p.age = Convert.ToInt32(fin.ReadLine());
                    p.count_leaves = Convert.ToInt32(fin.ReadLine());
                    p.hight = Convert.ToInt32(fin.ReadLine());
                    p.name = fin.ReadLine();
                    p.number_orangirea = i+10;
                    greenhouse_paporotnik.Add(p);
                }
            }

            using (StreamReader fin = new StreamReader("pixta.txt"))
            {
                count_plants = Convert.ToInt32(fin.ReadLine());

                for (int i = 0; i < count_plants; i++)
                {
                    var p = new pixta();
                    p.age = Convert.ToInt32(fin.ReadLine());
                    p.hight = Convert.ToInt32(fin.ReadLine());
                    p.number_orangirea = i + 20;
                    p.name = fin.ReadLine();
                    greenhouse_pixta.Add(p);
                }
            }

            return "Sucsessful";
        }

        // GETapi/values/Add/pixta/{name,age}
        [HttpGet("Add/pixta/{name}/{age}")]
        public string Add_pixta(string name, int age)
        {
                    var p = new pixta();
                    p.age = age;
                    p.name = name;
                    greenhouse_pixta[greenhouse_pixta.Count()] = p;

            return "New pixta was added";
        }

        // GETapi/values/Add/paporotnik/{name,age}
        [HttpGet("Add/paporotnik/{name}/{age}")]
        public string Add_paporotnik(string name, int age)
        {
            var p = new paporotnik();
            p.age = age;
            p.name = name;
            greenhouse_paporotnik[greenhouse_paporotnik.Count()] = p;

            return "New paporotnik was added";
        }

        // GETapi/values/Print/All/Paporotnik
        [HttpGet("Print/All/Paporotnik")]
        public List<paporotnik> Print_all_paporotnik() {return greenhouse_paporotnik;}

        // GETapi/values/Print/All/pixta
        [HttpGet("Print/All/pixta")]
        public List<pixta> Print_all_pixta() { return greenhouse_pixta; }

        // GETapi/values/Print/Select/pixta
        [HttpGet("Print/Select/pixta")]
        public IEnumerable<string> Select_pixta() {
            return greenhouse_pixta.Select(pixta => pixta.name);
        }

        // GETapi/values/Print/Select/paporotnik
        [HttpGet("Print/Select/paporotnik")]
        public IEnumerable<string> Select_paporotnik()
        {
            return greenhouse_paporotnik.Select(p => p.name);
        }

        // GETapi/values/Print/Where
        [HttpGet("Print/Where")]
        public IEnumerable<pixta> Print_where()
        {
            return greenhouse_pixta.Where(p => p.name.StartsWith("P"));
        }

        // GETapi/values/Print/Where
        [HttpGet("Print/Where1")]
        public IEnumerable<pixta> Print_where1()
        {
            return greenhouse_pixta.Where(p => p.name.StartsWith("P") && p.hight < 300);
        }

        // GETapi/values/Print/join
        [HttpGet("Print/join")]
        public List<string> Print_join()
        {
            var result = from paporot in greenhouse_paporotnik
                         join pix in greenhouse_pixta on paporot.hight equals pix.hight
                         select new { Name_paporot = paporot.name, Name_pixta = pix.name, Hight = paporot.hight };

            List<string> List_result = new List<string>();
            foreach (var item in result)
            List_result.Add(item.Name_paporot+"  "+item.Name_pixta+"  Hight = "+item.Hight.ToString());

            return List_result;
        }

        // GETapi/values/Print/orderby
        [HttpGet("Print/orderby")]
        public IEnumerable<pixta> Print_orderby()
        {
            return greenhouse_pixta.OrderBy(p => p.name) ;
        }


        //// GETapi/values/5
        //[HttpGet("Add/{name}/{age}")]
        //public List<paporotnik> Add(string name, int age)
        //{
        //   for (int i = 1; i <= age; i++)
        //    {
        //        var p = new paporotnik();
        //        greenhouse_paporotnik[i]=p;

        //    }
        //    return greenhouse_paporotnik.Values.Where(p=>p.age<3).ToList();
        //}

        // POST api/values
        [HttpPost]
        public void Post([FromBody] string value)
        {
        }

        // PUT api/values/5
        [HttpPut("{id}")]
        public void Put(int id, [FromBody] string value)
        {
        }

        // DELETE api/values/Delete
        [HttpDelete("/Delete")]
        public void Delete(int id)
        {
            greenhouse_pixta.Clear();
            greenhouse_paporotnik.Clear();
        }
    }
}
