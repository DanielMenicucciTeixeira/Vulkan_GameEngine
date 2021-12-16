using System;
using System.Runtime.InteropServices;
using System.Text;
using UnityEngine;


public class TestDLL : MonoBehaviour {
    [DllImport("TestDLL", CallingConvention = CallingConvention.Cdecl)]
    public static extern float Random();

    [DllImport("TestDLL", CallingConvention = CallingConvention.Cdecl)]
    public static extern float Move();

    [DllImport("TestDLL", CallingConvention = CallingConvention.Cdecl)]
    public static extern void Sort(int[] a, int length);

    float i = 0;
    public int[] a;

    void Start() {
        Sort(a, a.Length);
        foreach (var i in a) {
            print(i);
        }
        transform.position = new Vector3(Move(), transform.position.y, transform.position.z);
    }

    void FixedUpdate() {
        if(i % 100 == 1) {
            float x = Random() + 1;
            gameObject.transform.localScale = new Vector3(x, x, x);
        }
        i++;
    }
}
